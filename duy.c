```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_URL 100
#define MAX_TIME 20

typedef struct Operation {
    char url[MAX_URL];
    char timestamp[MAX_TIME];
    struct Operation* next;
    struct Operation* prev;
} Operation;

typedef struct Stack {
    Operation* top;
} Stack;

typedef struct DoublyLinkedList {
    Operation* head;
    Operation* tail;
} DoublyLinkedList;

void getCurrentTime(char* buffer) {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, MAX_TIME, "%Y-%m-%d %H:%M:%S", timeinfo);
}

Operation* createOperation(char* url) {
    Operation* op = (Operation*)malloc(sizeof(Operation));
    strncpy(op->url, url, MAX_URL - 1);
    op->url[MAX_URL - 1] = '\0';
    getCurrentTime(op->timestamp);
    op->next = NULL;
    op->prev = NULL;
    return op;
}

void initStack(Stack* stack) {
    stack->top = NULL;
}

void push(Stack* stack, Operation* op) {
    op->next = stack->top;
    stack->top = op;
}

Operation* pop(Stack* stack) {
    if (!stack->top) return NULL;
    Operation* temp = stack->top;
    stack->top = temp->next;
    temp->next = NULL;
    return temp;
}

void clearStack(Stack* stack) {
    while (stack->top) {
        Operation* temp = pop(stack);
        free(temp);
    }
}

void initList(DoublyLinkedList* list) {
    list->head = NULL;
    list->tail = NULL;
}

void appendList(DoublyLinkedList* list, Operation* op) {
    if (!list->head) {
        list->head = list->tail = op;
    } else {
        op->prev = list->tail;
        list->tail->next = op;
        list->tail = op;
    }
}

void clearList(DoublyLinkedList* list) {
    Operation* current = list->head;
    while (current) {
        Operation* temp = current;
        current = current->next;
        free(temp);
    }
    list->head = list->tail = NULL;
}

void visit(Stack* back, Stack* forward, DoublyLinkedList* history, char* url) {
    Operation* op = createOperation(url);
    push(back, op);
    appendList(history, createOperation(url));
    clearStack(forward);
    printf("truy cap thanh cong %s\n", url);
}

void backward(Stack* back, Stack* forward, DoublyLinkedList* history) {
    Operation* current = pop(back);
    if (!current) {
        printf("khong the quay lai\n");
        return;
    }
    push(forward, current);
    if (back->top) {
        printf("quay lai %s\n", back->top->url);
    } else {
        printf("khong co trang truoc do\n");
    }
}

void forward(Stack* back, Stack* forward, DoublyLinkedList* history) {
    Operation* current = pop(forward);
    if (!current) {
        printf("khong the di toi\n");
        return;
    }
    push(back, current);
    printf("di toi %s\n", current->url);
}

void current(Stack* back) {
    if (!back->top) {
        printf("khong co trang web dang xem\n");
        return;
    }
    printf("trang hien tai %s\n", back->top->url);
}

void printHistory(DoublyLinkedList* history) {
    if (!history->head) {
        printf("lich su trong\n");
        return;
    }
    Operation* current = history->head;
    while (current) {
        printf("%s %s\n", current->timestamp, current->url);
        current = current->next;
    }
}

int getCommandIndex(char* command) {
    if (strcmp(command, "VISIT") == 0) return 1;
    if (strcmp(command, "BACKWARD") == 0) return 2;
    if (strcmp(command, "FORWARD") == 0) return 3;
    if (strcmp(command, "CURRENT") == 0) return 4;
    if (strcmp(command, "HISTORY") == 0) return 5;
    if (strcmp(command, "CLEAR") == 0) return 6;
    if (strcmp(command, "THOAT") == 0) return 7;
    return 0;
}

int main() {
    Stack back, forward;
    DoublyLinkedList history;
    initStack(&back);
    initStack(&forward);
    initList(&history);
    
    char command[20], url[MAX_URL];
    
    while (1) {
        scanf("%s", command);
        int cmdIndex = getCommandIndex(command);
        
        switch (cmdIndex) {
            case 1:
                scanf("%s", url);
                visit(&back, &forward, &history, url);
                break;
            case 2:
                backward(&back, &forward, &history);
                break;
            case 3: 
                forward(&back, &forward, &history);
                break;
            case 4:
                current(&back);
                break;
            case 5: 
                printHistory(&history);
                break;
            case 6: 
                clearList(&history);
                printf("lich su da xoa\n");
                break;
            case 7: 
                clearStack(&back);
                clearStack(&forward);
                clearList(&history);
                printf("thoat chuong trinh\n");
                return 0;
            default:
                printf("lenh khong hop le\n");
                break;
        }
    }
    
    return 0;
}
```
