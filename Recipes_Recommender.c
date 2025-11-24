#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_RECIPES 50
#define MAX_NAME 50

int graph[MAX_NODES][MAX_NODES];
char ingredientList[MAX_NODES][MAX_NAME];
int ingredientCount = 0;

typedef struct {
    char name[MAX_NAME];
    int ingredients[20];
    int ingCount;
} Recipe;

Recipe recipes[MAX_RECIPES];
int recipeCount = 0;

int getIngredientIndex(char *name) {
    for (int i = 0; i < ingredientCount; i++)
        if (strcmp(ingredientList[i], name) == 0)
            return i;

    return -1;
}

int addIngredient(char *name) {
    int idx = getIngredientIndex(name);
    if (idx != -1) return idx;

    strcpy(ingredientList[ingredientCount], name);
    return ingredientCount++;
}

void addRelation(char *a, char *b) {
    int A = addIngredient(a);
    int B = addIngredient(b);
    graph[A][B] = graph[B][A] = 1;
}

void addRecipe(char *name, char ing[][MAX_NAME], int count) {
    strcpy(recipes[recipeCount].name, name);
    recipes[recipeCount].ingCount = count;

    for (int i = 0; i < count; i++)
        recipes[recipeCount].ingredients[i] = addIngredient(ing[i]);

    recipeCount++;
}

int bfs(int start, int target) {
    int q[MAX_NODES], f = 0, r = 0;
    int visited[MAX_NODES] = {0};

    q[r++] = start;
    visited[start] = 1;

    while (f < r) {
        int node = q[f++];

        if (node == target)
            return 1;

        for (int i = 0; i < ingredientCount; i++) {
            if (graph[node][i] && !visited[i]) {
                visited[i] = 1;
                q[r++] = i;
            }
        }
    }
    return 0;
}

float matchPercentage(Recipe r, int userIngIndex[], int userCount) {
    int match = 0;

    for (int i = 0; i < r.ingCount; i++) {
        int needed = r.ingredients[i];

        for (int j = 0; j < userCount; j++) {
            int user = userIngIndex[j];

            if (user == needed || bfs(user, needed)) {
                match++;
                break;
            }
        }
    }

    return (match * 100.0) / r.ingCount;
}

int main() {

    /* Ingredients */
    addIngredient("egg");
    addIngredient("milk");
    addIngredient("flour");
    addIngredient("sugar");
    addIngredient("butter");
    addIngredient("cheese");

    /* Relations */
    addRelation("milk", "cheese");

    /* Recipes */
    char pancakeIng[][MAX_NAME] = {"egg", "milk", "flour", "sugar"};
    addRecipe("Pancakes", pancakeIng, 4);

    char omeletteIng[][MAX_NAME] = {"egg", "milk", "cheese"};
    addRecipe("Cheese Omelette", omeletteIng, 3);

    /* User input */
    int n;
    printf("Enter number of available ingredients: ");
    scanf("%d", &n);

    char userIng[n][MAX_NAME];
    int userIngIndex[n];

    printf("Enter ingredients:\n");
    for (int i = 0; i < n; i++) {
        scanf("%s", userIng[i]);
        userIngIndex[i] = addIngredient(userIng[i]);
    }

    /* Recommend recipes */
    printf("\nRecommended Recipes (50%% or more match):\n");

    int any = 0;
    for (int i = 0; i < recipeCount; i++) {
        float percent = matchPercentage(recipes[i], userIngIndex, n);
        if (percent >= 50) {
            printf("✔ %s (%.1f%% match)\n", recipes[i].name, percent);
            any = 1;
        }
    }

    if (!any)
        printf("No matching recipes found.\n");

    return 0;
}
