#include <stdio.h>
#include <stdlib.h>
#include <JavaScriptCore/JavaScript.h>

// Structure for a custom function (not currently used in the code)
// typedef struct {
//     JSStringRef functionName;
//     JSObjectRef functionObject;
// } CustomFunction;

// Function to create a custom JavaScript function
void createCustomFunction(JSContextRef context, JSObjectRef globalObject, const char* functionName, JSObjectCallAsFunctionCallback functionCallback) {
    // Convert the function name to a JavaScript string
    JSStringRef functionString = JSStringCreateWithUTF8CString(functionName);

    // Create a JavaScript function object with the specified callback
    JSObjectRef functionObject = JSObjectMakeFunctionWithCallback(context, functionString, functionCallback);

    // Set the function object as a property of the global object in the JavaScript context
    JSObjectSetProperty(context, globalObject, functionString, functionObject, kJSPropertyAttributeNone, NULL);

    // Release the function name string
    JSStringRelease(functionString);
    // Return the CustomFunction structure (commented out - not used)
    // CustomFunction customFunc = { functionString, functionObject };
    // return customFunc;
}

// Callback function for adding two numbers in JavaScript
JSValueRef Add(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
    // Check if enough arguments are passed
    if (argumentCount < 2 || arguments == NULL) {
        fprintf(stderr, "The function requires 2 arguments.\n");
        return JSValueMakeUndefined(context);
    }

    // Convert JavaScript values to numbers
    int num1 = JSValueToNumber(context, arguments[0], NULL);
    int num2 = JSValueToNumber(context, arguments[1], NULL);

    // Perform addition and return the result as a JavaScript number
    int add = num1 + num2;
    printf("%d\n", add); // Print the result to the console
    return JSValueMakeNumber(context, add);
}

// Callback function for multiplying two numbers in JavaScript
JSValueRef Mult(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
    // Check if enough arguments are passed
    if (argumentCount < 2) {
        fprintf(stderr, "The function requires 2 arguments.\n");
        return JSValueMakeUndefined(context);
    }

    // Convert JavaScript values to numbers
    int numa = JSValueToNumber(context, arguments[0], NULL);
    int numb = JSValueToNumber(context, arguments[1], NULL);

    // Perform multiplication and return the result as a JavaScript number
    int mul = numa * numb;
    printf("%d\n", mul); // Print the result to the console
    return JSValueMakeNumber(context, mul);
}

// Function to read the content of a file
char * readScript(const char * filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Failed to open the file.\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = (char *)malloc(length + 1);
    fread(buffer, 1, length, file);
    fclose(file);
    buffer[length] = '\0';
    return buffer;
}

int main() {
    // Create a global JavaScript context
    JSGlobalContextRef context = JSGlobalContextCreate(NULL);
    JSObjectRef globalObject = JSContextGetGlobalObject(context);

    // Create and register custom JavaScript functions Add and Mult
    createCustomFunction(context, globalObject, "Add", Add);
    createCustomFunction(context, globalObject, "Mult", Mult);

    // Load JavaScript file content
    const char* filename = "index.js";
    char* fileContent = readScript(filename);

    if (fileContent == NULL) {
        return 1;
    }

    // Create a JavaScript string from the file content
    JSStringRef script = JSStringCreateWithUTF8CString(fileContent);

    // Evaluate the JavaScript script in the context
    JSEvaluateScript(context, script, NULL, NULL, 0, NULL);

    // Free memory and release resources
    free(fileContent);
    JSStringRelease(script);
    JSGlobalContextRelease(context);

    return 0;
}
