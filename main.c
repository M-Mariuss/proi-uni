#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Player {
    char* firstName;
    char* secondName;
    int points;
};

struct Team {
    int members;
    char* name;
    float teamScore;
    struct Player* players;
    struct Team* next;
};

struct Stack {
    struct Team* team;
    struct Stack* next;
};

struct Match {
    struct Team* team1;
    struct Team* team2;
    struct Match* next;
};

struct MatchQueue {
    struct Match* front;
    struct Match* rear;
};

struct Node {
    struct Team* team;
    float score;
    struct Node* left;
    struct Node* right;
};

void addTeam(struct Team** head, struct Team* newTeam) {

    newTeam->next = *head;
    *head = newTeam;
}

void removeSmallest(struct Team** head) {

    if (!*head) return;

    struct Team *curTeam = *head;
    struct Team *prev = NULL;
    struct Team *minNode = *head;
    struct Team *prevMin = NULL;

    while (curTeam != NULL) {

        if (curTeam->teamScore < minNode->teamScore) {

            minNode = curTeam;
            prevMin = prev;
        }

        prev = curTeam;
        curTeam = curTeam->next;
    }

    if (prevMin != NULL) {

        prevMin->next = minNode->next;
    }
    else {

        *head = minNode->next;
    }

    for (int i = 0; i < minNode->members; i++) {

        free(minNode->players[i].firstName);
        free(minNode->players[i].secondName);
    }

    free(minNode->name);
    free(minNode->players);
    free(minNode);
}

void push(struct Stack** top, struct Team* team) {

    struct Stack *newNode = malloc(sizeof(struct Stack));
    newNode->team = team;
    newNode->next = *top;
    *top = newNode;
}

struct Team* pop(struct Stack** top) {

    if (*top == NULL) return NULL;

    struct Stack *temp = *top;
    struct Team *team = temp->team;
    *top = (*top)->next;
    free(temp);

    return team;
}

int isEmptyStack(struct Stack* top) {

    return top == NULL;
}

struct MatchQueue* createQueue() {

    struct MatchQueue *q = malloc(sizeof(struct MatchQueue));
    q->front = q->rear = NULL;

    return q;
}

void enQueue(struct MatchQueue* q, struct Team* t1, struct Team* t2) {

    struct Match *newMatch = malloc(sizeof(struct Match));
    newMatch->team1 = t1;
    newMatch->team2 = t2;
    newMatch->next = NULL;

    if (q->rear == NULL) {

        q->rear = newMatch;
    }
    else {

        q->rear->next = newMatch;
        q->rear = newMatch;
    }

    if (q->front == NULL) q->front = q->rear;
}

struct Match* deQueue(struct MatchQueue* q) {

    if (q->front == NULL) return NULL;

    struct Match *temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL) q->rear = NULL;

    return temp;
}

int isEmptyQueue(struct MatchQueue* q) {

    return q->front == NULL;
}

struct Node* bstNewNode(struct Team* team, float score) {

    struct Node* node = malloc(sizeof(struct Node));
    node->team = team;
    node->score = score;
    node->left = node->right = NULL;
    return node;
}

struct Node* bstInsert(struct Node* node, struct Team* team, float score) {

    if (node == NULL) return bstNewNode(team, score);

    if (score > node->score) {

        node->right = bstInsert(node->right, team, score);
    }
    else if (score < node->score) {

        node->left = bstInsert(node->left, team, score);
    }
    else {

        if (strcmp(team->name, node->team->name) > 0) {

            node->right = bstInsert(node->right, team, score);
        }
        else {

            node->left = bstInsert(node->left, team, score);
        }
    }

    return node;
}

void reverseInorder(struct Node* node, FILE* Output) {

    if (node == NULL) return;

    reverseInorder(node->right, Output);
    fprintf(Output, "%-33s -  %.2f\n", node->team->name, node->score);
    reverseInorder(node->left, Output);
}

int main(int argc, char* argv[]) {

    if (argc < 4) {

        return -1;
    }

    FILE *Input, *Cerinta, *Output;

    Cerinta = fopen(argv[1], "r");
    Input = fopen(argv[2], "r");
    Output = fopen(argv[3], "w");

    if (Input == NULL || Cerinta == NULL || Output == NULL) {

        printf("Eroare\n");
        return -2;
    }

    int tasks[4];
    for (int i = 0; i < 4; i++) {

        fscanf(Cerinta, "%d", &tasks[i]);
    }

    struct Team* head = NULL;

    int teamCount;
    fscanf(Input, "%d ", &teamCount);
    for (int i = 0; i < teamCount; i++) {

        struct Team* newTeam = malloc(sizeof(struct Team));
        fscanf(Input, "%d ", &newTeam->members);

        char buffer[128];
        if (fgets(buffer, 128, Input) != NULL) {

            int len = strlen(buffer);
            while (len > 0) {

                if (!isspace(buffer[len - 1])) {

                    break;
                }

                len--;
            }
            buffer[len] = '\0';

            newTeam->name = malloc(len + 1);
            strcpy(newTeam->name, buffer);
        }

        newTeam->players = malloc(newTeam->members * sizeof(struct Player));

        float sum = 0.0f;
        for (int j = 0; j < newTeam->members; j++) {

            char firstName[32], secondName[32];
            int points;

            fscanf(Input, "%s %s %d ", firstName, secondName, &points);
            newTeam->players[j].firstName = malloc(strlen(firstName) + 1);
            strcpy(newTeam->players[j].firstName, firstName);
            newTeam->players[j].secondName = malloc(strlen(secondName) + 1);
            strcpy(newTeam->players[j].secondName, secondName);
            newTeam->players[j].points = points;

            sum += points;
        }

        newTeam->teamScore = sum / newTeam->members;
        addTeam(&head, newTeam);
    }

    if (tasks[0] == 1) {

        int nearest = 1;
        while (nearest * 2 <= teamCount) {

            nearest *= 2;
        }

        int difference = teamCount - nearest;
        for (int i = 0; i < difference; i++) {

            removeSmallest(&head);
        }
    }

    if (tasks[1] == 1) {

        struct Team* top8[8];
        float top8Scores[8];
        int top8Count = 0;

        struct Team *curr = head;

        while (curr != NULL) {

            fprintf(Output, "%s\n", curr->name);
            curr = curr->next;
        }

        fprintf(Output, "\n");

        int teamsLeft = 0;
        curr = head;

        while (curr != NULL) {

            teamsLeft++;
            curr = curr->next;
        }

        struct MatchQueue *q = createQueue();
        curr = head;

        while (curr != NULL && curr->next != NULL) {

            enQueue(q, curr, curr->next);
            curr = curr->next->next;
        }

        int roundNo = 1;

        while (teamsLeft > 1) {

            fprintf(Output, "--- ROUND NO:%d\n", roundNo);

            struct Match *m = q->front;

            while (m != NULL) {

                fprintf(Output, "%-32s - %32s\n", m->team1->name, m->team2->name);
                m = m->next;
            }

            fprintf(Output, "\n");
            fprintf(Output, "WINNERS OF ROUND NO:%d\n", roundNo);

            struct Stack *stack = NULL;

            while (!isEmptyQueue(q)) {

                struct Match *match = deQueue(q);
                struct Team *winner;

                if (match->team1->teamScore > match->team2->teamScore) {

                    winner = match->team1;
                }
                else {

                    winner = match->team2;
                }

                float sum = 0.0f;
                for (int i = 0; i < winner->members; i++) {

                    winner->players[i].points++;
                    sum += winner->players[i].points;
                }

                winner->teamScore = sum / winner->members;
                push(&stack, winner);
                free(match);
            }

            teamsLeft /= 2;

            if (teamsLeft == 8) {

                struct Stack *s = stack;
                while (s != NULL) {

                    top8[top8Count] = s->team;
                    top8Scores[top8Count] = s->team->teamScore;
                    top8Count++;
                    s = s->next;
                }
            }

            struct Stack *s = stack;

            while (s != NULL) {

                fprintf(Output, "%-33s -  %.2f\n", s->team->name, s->team->teamScore);
                s = s->next;
            }

            while (!isEmptyStack(stack)) {

                struct Team *t1 = pop(&stack);
                struct Team *t2 = pop(&stack);

                if (t2 != NULL) {

                    enQueue(q, t1, t2);
                }
            }

            if (teamsLeft > 1) {

                fprintf(Output, "\n");
            }

            roundNo++;
        }

        if (tasks[2] == 1) {

            struct Node* bstRoot = NULL;
            for (int i = 0; i < top8Count; i++) {

                bstRoot = bstInsert(bstRoot, top8[i], top8Scores[i]);
            }

            fprintf(Output, "\nTOP 8 TEAMS:\n");
            reverseInorder(bstRoot, Output);
        }

        free(q);
    }
    else {

        struct Team *curr = head;

        while (curr != NULL) {

            fprintf(Output, "%s\n", curr->name);
            curr = curr->next;
        }
    }

    fclose(Input);
    fclose(Cerinta);
    fclose(Output);
    return 0;
}
