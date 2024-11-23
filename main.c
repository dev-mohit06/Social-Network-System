#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_VERTICES 100
#define MAX_NAME 50
#define INF 999999


typedef struct {
    char vertices[MAX_VERTICES][MAX_NAME];     // Store vertex (user) names
    int adjMatrix[MAX_VERTICES][MAX_VERTICES]; // Adjacency matrix for connections
    int vertexCount;                           // Number of vertices in graph
} Graph;

void initGraph(Graph* g) {
    g->vertexCount = 0;
    for(int i = 0; i < MAX_VERTICES; i++) {
        for(int j = 0; j < MAX_VERTICES; j++) {
            g->adjMatrix[i][j] = 0;
        }
    }
}

int addVertex(Graph* g, const char* name) {
    if(g->vertexCount >= MAX_VERTICES) return -1;
    strcpy(g->vertices[g->vertexCount], name);
    return g->vertexCount++;
}

int findVertex(Graph* g, const char* name) {
    for(int i = 0; i < g->vertexCount; i++) {
        if(strcmp(g->vertices[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

void addEdge(Graph* g, const char* v1, const char* v2) {
    int id1 = findVertex(g, v1);
    int id2 = findVertex(g, v2);
    
    if(id1 != -1 && id2 != -1) {
        g->adjMatrix[id1][id2] = 1;
        g->adjMatrix[id2][id1] = 1;  // Undirected graph
    }
}

void printGraph(Graph* g) {
    printf("\nGraph Structure:\n");
    for(int i = 0; i < g->vertexCount; i++) {
        printf("%s: ", g->vertices[i]);
        bool first = true;
        for(int j = 0; j < g->vertexCount; j++) {
            if(g->adjMatrix[i][j]) {
                if(!first) printf(", ");
                printf("%s", g->vertices[j]);
                first = false;
            }
        }
        printf("\n");
    }
}

void bfs(Graph* g, int start, int* distance, int* parent) {
    bool* visited = (bool*) calloc(g->vertexCount, sizeof(bool));
    int* queue = (int*)malloc(g->vertexCount * sizeof(int));
    int front = 0, rear = 0;
    
    // Initialize distances and parents
    for(int i = 0; i < g->vertexCount; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }
    
    // Start BFS
    distance[start] = 0;
    visited[start] = true;
    queue[rear++] = start;
    
    while(front < rear) {
        int current = queue[front++];
        // Check all adjacent vertices
        for(int i = 0; i < g->vertexCount; i++) {
            if(g->adjMatrix[current][i] && !visited[i]) {
                visited[i] = true;
                distance[i] = distance[current] + 1;
                parent[i] = current;
                queue[rear++] = i;
            }
        }
    }
    
    free(visited);
    free(queue);
}

// Find mutual friends (common neighbors in graph)
void findMutualFriends(Graph* g, const char* v1, const char* v2) {
    int id1 = findVertex(g, v1);
    int id2 = findVertex(g, v2);
    
    if(id1 == -1 || id2 == -1) {
        printf("One or both vertices not found!\n");
        return;
    }
    
    printf("\nMutual friends between %s and %s:\n", v1, v2);
    bool found = false;
    
    for(int i = 0; i < g->vertexCount; i++) {
        if(g->adjMatrix[id1][i] && g->adjMatrix[id2][i]) {
            printf("- %s\n", g->vertices[i]);
            found = true;
        }
    }
    
    if(!found) printf("No mutual friends found.\n");
}

void shortestPath(Graph* g, const char* start, const char* end) {
    int startId = findVertex(g, start);
    int endId = findVertex(g, end);
    
    if(startId == -1 || endId == -1) {
        printf("One or both vertices not found!\n");
        return;
    }
    
    int* distance = (int*)malloc(g->vertexCount * sizeof(int));
    int* parent = (int*)malloc(g->vertexCount * sizeof(int));
    
    bfs(g, startId, distance, parent);
    
    if(distance[endId] == INF) {
        printf("\nNo path exists between %s and %s\n", start, end);
    } else {
        // Reconstruct path
        int* path = (int*)malloc(g->vertexCount * sizeof(int));
        int pathLen = 0;
        int current = endId;
        
        while(current != -1) {
            path[pathLen++] = current;
            current = parent[current];
        }
        
        printf("\nShortest path from %s to %s:\n", start, end);
        for(int i = pathLen - 1; i >= 0; i--) {
            printf("%s", g->vertices[path[i]]);
            if(i > 0) printf(" -> ");
        }
        printf("\n");
        
        free(path);
    }
    
    free(distance);
    free(parent);
}

// Function to get friend recommendations for a user within specified degrees of separation
void recommendFriends(Graph* g, const char* username, int degree) {
    int start = findVertex(g, username);
    if (start == -1) {
        printf("User %s not found.\n", username);
        return;
    }

    // Arrays to track visited vertices and their distances
    int visited[MAX_VERTICES] = {0};
    int distances[MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++) {
        distances[i] = INT_MAX;
    }
    
    // Use BFS to find vertices within specified degree
    int queue[MAX_VERTICES];
    int front = 0, rear = 0;
    
    queue[rear++] = start;
    distances[start] = 0;
    visited[start] = 1;
    
    printf("Friend recommendations for %s within %d levels:\n", username, degree);
    int found = 0;

    while (front < rear) {
        int current = queue[front++];
        
        // If we've exceeded the degree limit, stop processing this path
        if (distances[current] >= degree) {
            continue;
        }
        
        // Check all adjacent vertices
        for (int i = 0; i < g->vertexCount; i++) {
            if (g->adjMatrix[current][i] && !visited[i]) {
                visited[i] = 1;
                distances[i] = distances[current] + 1;
                queue[rear++] = i;
                
                // If this is a connection within our degree limit and not already a direct friend
                if (distances[i] <= degree && !g->adjMatrix[start][i]) {
                    printf("- %s\n", g->vertices[i]);
                    found = 1;
                }
            }
        }
    }
    
    if (!found) {
        printf("No recommendations found within %d degrees.\n", degree);
    }
}

// Function to identify and list users by popularity (connection count)
void mostPopular(Graph* g) {
    // Create array of user indices and their connection counts
    typedef struct {
        int index;
        int connections;
    } UserPopularity;
    
    UserPopularity users[MAX_VERTICES];
    
    // Count connections for each user
    for (int i = 0; i < g->vertexCount; i++) {
        users[i].index = i;
        users[i].connections = 0;
        for (int j = 0; j < g->vertexCount; j++) {
            if (g->adjMatrix[i][j]) {
                users[i].connections++;
            }
        }
    }
    
    // Sort users by connection count (bubble sort for simplicity)
    for (int i = 0; i < g->vertexCount - 1; i++) {
        for (int j = 0; j < g->vertexCount - i - 1; j++) {
            if (users[j].connections < users[j + 1].connections) {
                UserPopularity temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }
    
    printf("Most popular users:\n");
    for (int i = 0; i < g->vertexCount; i++) {
        if (users[i].connections > 0) {
            printf("- %s (%d connections)\n", 
                   g->vertices[users[i].index], 
                   users[i].connections);
        }
    }
}

void listFriends(Graph* g, const char* username) {
    int userIndex = findVertex(g, username);
    if (userIndex == -1) {
        printf("User %s not found.\n", username);
        return;
    }
    
    printf("Friends of %s:\n", username);
    int found = 0;
    
    for (int i = 0; i < g->vertexCount; i++) {
        if (g->adjMatrix[userIndex][i]) {
            printf("- %s\n", g->vertices[i]);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No friends found.\n");
    }
}


int main()
{
    Graph graph;
    initGraph(&graph);

    // Read vertices
    int numVertices;
    char input_buffer[1000];  // Buffer for entire input line
    
    while (1) {
        printf("Enter number of users: ");
        if (scanf("%d", &numVertices) == 1 && numVertices > 0 && numVertices <= MAX_VERTICES) {
            break;
        }
        printf("Please enter a valid number between 1 and %d\n", MAX_VERTICES);
        // Clear input buffer
        while (getchar() != '\n');
    }
    while (getchar() != '\n');  // Clear the newline
    
    printf("Enter user names (one per line):\n");
    char name[MAX_NAME];
    for (int i = 0; i < numVertices; i++)
    {
        printf("User %d: ", i + 1);
        if (fgets(name, MAX_NAME, stdin)) {
            // Remove newline if present
            name[strcspn(name, "\n")] = 0;
            if (strlen(name) > 0) {
                addVertex(&graph, name);
            }
        }
    }

    // Read edges
    int numEdges;
    while (1) {
        printf("Enter number of friendships: ");
        if (scanf("%d", &numEdges) == 1 && numEdges >= 0 && 
            numEdges <= (numVertices * (numVertices - 1) / 2)) {
            break;
        }
        printf("Please enter a valid number between 0 and %d\n", 
               (numVertices * (numVertices - 1) / 2));
        while (getchar() != '\n');
    }
    while (getchar() != '\n');  // Clear the newline

    printf("Enter friendships (one pair per line, separated by space):\n");
    char v1[MAX_NAME], v2[MAX_NAME];
    for (int i = 0; i < numEdges; i++)
    {
        printf("Friendship %d: ", i + 1);
        if (fgets(input_buffer, sizeof(input_buffer), stdin)) {
            if (sscanf(input_buffer, "%s %s", v1, v2) == 2) {
                addEdge(&graph, v1, v2);
            } else {
                printf("Invalid input format. Please enter two names separated by space.\n");
                i--; // Retry this friendship
            }
        }
    }

    // Command processing
    char command[50];
    printf("\nAvailable commands:\n");
    printf("1. print_graph - Display the network\n");
    printf("2. mutual_friends <user1> <user2> - Find mutual friends\n");
    printf("3. shortest_path <user1> <user2> - Find shortest path\n");
    printf("4. recommend_friends <user> <degree> - Get friend recommendations\n");
    printf("5. most_popular - List users by popularity\n");
    printf("6. list_friends <user> - List friends of a user\n");
    printf("7. clear - Clear the entire screen\n");
    printf("8. exit - Exit the program\n\n");

    while (1)
    {
        printf("\nEnter command: ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin)) {
            input_buffer[strcspn(input_buffer, "\n")] = 0;  // Remove newline

            if (strlen(input_buffer) == 0) {
                continue;
            }

            char *cmd = strtok(input_buffer, " ");
            if (!cmd) continue;

            if (strcmp(cmd, "exit") == 0)
            {
                break;
            }
            else if (strcmp(cmd, "print_graph") == 0)
            {
                printGraph(&graph);
            }
            else if (strcmp(cmd, "mutual_friends") == 0)
            {
                char *user1 = strtok(NULL, " ");
                char *user2 = strtok(NULL, " ");
                if (user1 && user2) {
                    findMutualFriends(&graph, user1, user2);
                } else {
                    printf("Usage: mutual_friends <user1> <user2>\n");
                }
            }
            else if (strcmp(cmd, "shortest_path") == 0)
            {
                char *user1 = strtok(NULL, " ");
                char *user2 = strtok(NULL, " ");
                if (user1 && user2) {
                    shortestPath(&graph, user1, user2);
                } else {
                    printf("Usage: shortest_path <user1> <user2>\n");
                }
            }
            else if (strcmp(cmd, "recommend_friends") == 0)
            {
                char *user = strtok(NULL, " ");
                char *deg_str = strtok(NULL, " ");
                if (user && deg_str) {
                    int degree = atoi(deg_str);
                    if (degree > 0) {
                        recommendFriends(&graph, user, degree);
                    } else {
                        printf("Degree must be a positive number\n");
                    }
                } else {
                    printf("Usage: recommend_friends <user> <degree>\n");
                }
            }
            else if (strcmp(cmd, "most_popular") == 0)
            {
                mostPopular(&graph);
            }
            else if (strcmp(cmd, "list_friends") == 0)
            {
                char *user = strtok(NULL, " ");
                if (user) {
                    listFriends(&graph, user);
                } else {
                    printf("Usage: list_friends <user>\n");
                }
            }
            else if (strcmp(cmd, "clear") == 0)
            {
                system("clear");
            }
            else
            {
                printf("Invalid command! Type 'help' for available commands.\n");
            }
        }
    }

    return 0;
}