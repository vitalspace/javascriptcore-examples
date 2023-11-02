#include <stdio.h>
#include <stdlib.h>
#include <JavaScriptCore/JavaScript.h>

// Callback function to log to the console
JSValueRef LogToConsoleCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
    for (size_t i = 0; i < argumentCount; ++i) {
        // Convert JavaScript value to string and print it to the console
        JSStringRef string = JSValueToStringCopy(ctx, arguments[i], NULL);
        size_t maxSize = JSStringGetMaximumUTF8CStringSize(string);
        char* utf8 = (char*)malloc(maxSize);
        JSStringGetUTF8CString(string, utf8, maxSize);
        printf("%s\n", utf8);
        free(utf8);
        JSStringRelease(string);
    }

    return JSValueMakeUndefined(ctx);
}

// Function to read file contents
char* readFileContents(const char* filename) {
    FILE* file = fopen(filename, "r");
   
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = (char*)malloc(length + 1);
    fread(buffer, 1, length, file);
    fclose(file);
    buffer[length] = '\0';

    return buffer;
}

int main() {
    // Creating a JavaScript execution context
    JSGlobalContextRef context = JSGlobalContextCreate(NULL);

    // Getting the global object
    JSObjectRef globalObject = JSContextGetGlobalObject(context);
    
    // Creating a 'console' object in the JavaScript context
    JSStringRef consoleProperty = JSStringCreateWithUTF8CString("console");
    JSObjectRef consoleObject = JSObjectMake(context, NULL, NULL);
    JSObjectSetProperty(context, globalObject, consoleProperty, consoleObject, kJSPropertyAttributeNone, NULL);

    // Creating a 'log' function and assigning the LogToConsoleCallback as its callback
    JSStringRef logProperty = JSStringCreateWithUTF8CString("log");
    JSObjectRef logFunction = JSObjectMakeFunctionWithCallback(context, logProperty, &LogToConsoleCallback);
    JSObjectSetProperty(context, consoleObject, logProperty, logFunction, kJSPropertyAttributeNone, NULL);

    const char* filename = "index.js";
    char* fileContents = readFileContents(filename);
    if (fileContents == NULL) {
        return 1;
    }

    // Creating a JS string with the file contents
    JSStringRef script = JSStringCreateWithUTF8CString(fileContents);
    JSValueRef exception = NULL;

    // Evaluating the JavaScript script in the context
    JSEvaluateScript(context, script, NULL, NULL, 1, &exception);

    if (exception) {
        // Handling exceptions if occurred during script evaluation
        JSStringRef exceptionMessage = JSValueToStringCopy(context, exception, NULL);
        size_t bufferSize = JSStringGetMaximumUTF8CStringSize(exceptionMessage);
        char* exceptionBuffer = (char*)malloc(bufferSize);
        JSStringGetUTF8CString(exceptionMessage, exceptionBuffer, bufferSize);
        printf("Error: %s\n", exceptionBuffer);

        free(exceptionBuffer);
        JSStringRelease(exceptionMessage);
    }

    // Free memory
    free(fileContents);
    JSStringRelease(script);
    JSGlobalContextRelease(context);

    return 0;
}
