#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node
{
    char* data;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

Node *newnode(char* val)
{
    Node *n = malloc(sizeof(Node));

    n->left = NULL;
    n->right = NULL;

    n->data = malloc(strlen(val) + 1);
    strcpy(n->data, val);

    n->height = 1;

    return n;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int height(Node *root)
{
    if(root == NULL) return 0;
    return root->height;
}

int getbalance(Node *root)
{
    if(root == NULL) return 0;

    return height(root->left) - height(root->right);
}

Node *rightrotate(Node *y)
{
    Node *x = y->left;
    Node *t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x;
}

Node *leftrotate(Node *x)
{
    Node *y = x->right;
    Node *t2 = y->left;

    y->left = x;
    x->right = t2;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

Node *insert(Node *root, char* x)
{
    if(root == NULL)
        return newnode(x);

    
    if(strcasecmp(x, root->data) < 0)
    {
        root->left = insert(root->left, x);
    }

    
    else if(strcasecmp(x, root->data) > 0)
    {
        root->right = insert(root->right, x);
    }

    else
    {
        return root;
    }

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getbalance(root);

  
    if(balance > 1 && strcasecmp(x, root->left->data) < 0)
        return rightrotate(root);

    
    if(balance < -1 && strcasecmp(x, root->right->data) > 0)
        return leftrotate(root);

    
    if(balance > 1 && strcasecmp(x, root->left->data) > 0)
    {
        root->left = leftrotate(root->left);
        return rightrotate(root);
    }

    
    if(balance < -1 && strcasecmp(x, root->right->data) < 0)
    {
        root->right = rightrotate(root->right);
        return leftrotate(root);
    }

    return root;
}

void inorder(Node *root)
{
    if(root == NULL) return;

    inorder(root->left);

    printf("%s ", root->data);

    inorder(root->right);
}

Node* search(Node* root, char* key, Node* prev) {
    if (root == NULL) return prev;
    int r = strcasecmp(key,root->data);
    if (r == 0) return root;
    else if (r > 0) return search(root->right, key, root);
    else if (r < 0) return search(root->left, key, root);
}

void findPredSucc(Node* root, char* key, Node** pred, Node** succ)
{
    *pred = NULL;
    *succ = NULL;

    while (root != NULL)
    {
        int r = strcasecmp(key, root->data);

        
        if (r == 0)
        {
            
            if (root->left != NULL)
            {
                Node* temp = root->left;
                while (temp->right != NULL)
                    temp = temp->right;

                *pred = temp;
            }

            
            if (root->right != NULL)
            {
                Node* temp = root->right;
                while (temp->left != NULL)
                    temp = temp->left;

                *succ = temp;
            }

            return;
        }


        else if (r > 0)
        {
            *pred = root;
            root = root->right;
        }


        else
        {
            *succ = root;
            root = root->left;
        }
    }
}

int main()
{
    printf("WELCOME TO THE SPELL CHECKER\n");
    printf("-------------------------\n\n");
    FILE* f;
    f = fopen("Dictionary.txt", "r");
    if (f == NULL) return 1;

    Node *root = NULL;
    int i = 0;
    char word[100];

    while(fscanf(f,"%99s", word) == 1) {
        root = insert(root, word);
        i++;
    }

    char str[100];
    
    printf("SUCCESSFULLY LOADED DICTIONARY\n");
    printf("SIZE: %d\n", i);
    printf("HEIGHT: %d\n\n", root->height);
    printf("-------------------------\n\n");
    printf("ENTER A SENTENCE:\n");
    fgets(str, 100, stdin);
    str[strcspn(str, "\n")] = '\0';
    char* tok = strtok(str," ");
    printf("\nRESULTS:\n");
    while(tok != NULL) {
        
        int found = 0;
        char sugg1[100];
        char sugg2[100];
        char sugg3[100];

        Node* result = search(root, tok, NULL);
        if (result != NULL && strcasecmp(tok, result->data) == 0) {
            found = 1;
        } else if (result != NULL) {
            strcpy(sugg1, result->data);
            Node* succ;
            Node* pred;

            findPredSucc(root, result->data, &pred, &succ);

            strcpy(sugg2,succ->data);
            strcpy(sugg3,pred->data);
        }

        if (found) printf("%s, CORRECT\n", tok);
        else {
            printf("%s, INCORRECT, Suggestions: %s %s %s\n", tok, sugg1, sugg2, sugg3);
        }

        tok = strtok(NULL, " ");
    }
    printf("-------------------------\n");
    fclose(f);

    return 0;
}