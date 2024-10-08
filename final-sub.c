#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h> 
#include <time.h>
#include <unistd.h>

#define NUM_VERTICES 15 // Number of locations

// Traffic condition enum
enum TrafficCondition {
    LOW,
    MEDIUM,
    HIGH
};

// Node structure for adjacency list
struct AdjListNode {
    int dest; // Destination vertex
    int weight; // Edge weight
    bool road_closed; // Indicates if the road is closed
    enum TrafficCondition traffic_condition; // Indicates the traffic condition
    struct AdjListNode* next; // Pointer to the next node in the adjacency list
};

// Structure to represent an adjacency list
struct AdjList {
    struct AdjListNode* head; // Pointer to the head of the list
};

// Structure to represent a graph
struct Graph {
    int V; // Number of vertices
    struct AdjList* array; // Array of adjacency lists
};

// Create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, int weight, bool road_closed, enum TrafficCondition traffic_condition) {
    struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->road_closed = road_closed;
    newNode->traffic_condition = traffic_condition;
    newNode->next = NULL;
    return newNode;
}

// Create a graph with V locations
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->array = (struct AdjList*)malloc(V * sizeof(struct AdjList));
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    return graph;
}

// Randomly generate a traffic condition (LOW, MEDIUM, or HIGH)
enum TrafficCondition randomTrafficCondition() {
    int rand_num = rand() % 3; // Generate a random number between 0 and 2
    return (enum TrafficCondition)rand_num;
}

// Add an edge to the graph considering road closures and traffic conditions
void addEdge(struct Graph* graph, int src, int dest, int weight, bool road_closed) {
    enum TrafficCondition traffic_condition = randomTrafficCondition(); // Generate random traffic condition
    struct AdjListNode* newNode = newAdjListNode(dest, weight, road_closed, traffic_condition);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Add the reverse edge (undirected graph)
    newNode = newAdjListNode(src, weight, road_closed, traffic_condition);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Update traffic conditions in the graph
void updateTrafficConditions(struct Graph* graph) {
    for (int i = 0; i < graph->V; ++i) {
        struct AdjListNode* pCrawl = graph->array[i].head;
        while (pCrawl != NULL) {
            pCrawl->traffic_condition = randomTrafficCondition(); // Update traffic condition
            pCrawl = pCrawl->next;
        }
    }
}

// Dijkstra's algorithm to find shortest paths from source to all other vertices
void dijkstra(struct Graph* graph, int src, bool ev_stations[], int* result_dist, int* result_path) {
    int dist[graph->V]; // Array to store the shortest distances
    bool sptSet[graph->V]; // Array to track vertices included in shortest path tree

    // Initialize distances and sptSet
    for (int i = 0; i < graph->V; ++i) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
        result_path[i] = -1; // Initialize result_path to -1
    }

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < graph->V - 1; ++count) {
        // Pick the minimum distance vertex from the set of vertices not yet processed
        int u, min_dist = INT_MAX;
        for (int v = 0; v < graph->V; ++v)
            if (!sptSet[v] && dist[v] <= min_dist)
                min_dist = dist[v], u = v;

        // Mark the picked vertex as processed
        sptSet[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->dest;
            if (!sptSet[v] && dist[u] != INT_MAX) {
                int alt_dist = dist[u] + pCrawl->weight;
                if (pCrawl->road_closed) {
                    // Road is closed, set distance to infinity
                    dist[v] = INT_MAX;
                } else if (alt_dist < dist[v]) {
                    // Update distance if a shorter path is found
                    dist[v] = alt_dist;
                    result_path[v] = u; // Store the predecessor of v
                }
            }
            pCrawl = pCrawl->next;
        }
    }

    // Store distances to EV stations in result_dist array
    for (int i = 0; i < graph->V; ++i) {
        if (ev_stations[i]) {
            result_dist[i] = dist[i];
        }
    }
}

// Print the shortest distance, path, EV station information, and traffic conditions
void printResult(int src, int* result_dist, int* result_path, bool* ev_stations, struct Graph* graph) {
    printf("Nearest EV station, its distance, path, and traffic condition from source node %d:\n", src);
    for (int i = 0; i < graph->V; ++i) {
        if (ev_stations[i]) {
            if (result_dist[i] < INT_MAX) {
                printf("EV station: %d, Distance: %d, Path: ", i, result_dist[i]);
                int node = i;
                int path[graph->V];
                int index = 0;
                while (node != src) {
                    path[index++] = node;
                    node = result_path[node];
                }
                path[index] = src;
                for (int j = index; j >= 0; --j) {
                    printf("%d", path[j]);
                    if (j > 0) printf(" -> ");
                }
                printf(", Traffic Condition: ");
                switch (graph->array[i].head->traffic_condition) {
                    case LOW:
                        printf("LOW\n");
                        break;
                    case MEDIUM:
                        printf("MEDIUM\n");
                        break;
                    case HIGH:
                        printf("HIGH\n");
                        break;
                    default:
                        printf("Traffic condition not available\n");
                        break;
                }
            } else {
                printf("For EV station: %d, No path found from your location %d\n", i, src);
            }
        }
    }
}

int main() {
    int V = NUM_VERTICES;
    struct Graph* graph = createGraph(V);

    // Example graph construction
    addEdge(graph, 0, 12, 60, false);   // Road open
    addEdge(graph, 1, 9, 350, false);   // Road open
    addEdge(graph, 1, 12, 700, false);  // Road open
    addEdge(graph, 2, 9, 400, false);   // Road open
    addEdge(graph, 2, 3, 190, false);   // Road open
    addEdge(graph, 2, 8, 300, false);   // Road open
    addEdge(graph, 3, 4, 260, false);   // Road open
    addEdge(graph, 3, 13, 400, false);  // Road open
    addEdge(graph, 4, 5, 240, false);   // Road open
    addEdge(graph, 4, 6, 280, false);   // Road open
    addEdge(graph, 5, 8, 70, false);    // Road closed
    addEdge(graph, 5, 10, 950, false);  // Road open
    addEdge(graph, 6, 13, 350, false);  // Road open
    addEdge(graph, 6, 14, 400, false);  // Road open
    addEdge(graph, 7, 14, 50, true);    // Road closed
    addEdge(graph, 7, 13, 500, true);   // Road closed
    addEdge(graph, 8, 11, 2800, false); // Road open
    addEdge(graph, 9, 13, 600, false);  // Road open
    addEdge(graph, 9, 12, 550, false);  // Road open
    addEdge(graph, 10, 14, 850, false); // Road open
    addEdge(graph, 11, 14, 1700, false);// Road open

    // Array indicating presence of EV stations
    bool ev_stations[V];
    for (int i = 0; i < V; i++) {
        ev_stations[i] = false;  // Initialize as no stations present
    }
    ev_stations[7] = true;  // EV station at 7
    ev_stations[3] = true;  // EV station at 3
    ev_stations[1] = true;  // EV station at 1
    ev_stations[8] = true;  // EV station at 8

    // Arrays to store distances to EV stations and paths
    int result_dist[V];
    int result_path[V];

    // Source node (current location)
    int src = 5;

    // Seed the random number generator
    srand(time(NULL));

    
    for (int i = 0; i < 5; ++i) { // Run for 5 iterations (25 seconds)
        // Update traffic conditions
        updateTrafficConditions(graph);

        // Apply Dijkstra's algorithm
        dijkstra(graph, src, ev_stations, result_dist, result_path);

        // Print the result
        printf("\nTime: After %d seconds\n", 5 * i);
        printResult(src, result_dist, result_path, ev_stations, graph);

        // Sleep for 5 seconds 
        sleep(5);
    }

    return 0;
}
