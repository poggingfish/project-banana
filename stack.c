typedef struct stack{
    int sp;
    int stack[100];
} stack;
stack stackInit(){
    stack Stack;
    Stack.sp = 0;
    return Stack;
}
void stackPush(int val, stack *Stack){
    Stack->stack[Stack->sp++] = val;
}
int stackPop(stack *Stack){
    return Stack->stack[--Stack->sp];
}
int peek(stack *Stack){
    return Stack->stack[Stack->sp];
}
int len(stack *Stack){
    return Stack->sp;
}