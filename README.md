# EV-Charging-Station-Locator

This project implements a traffic-aware shortest path finder using Dijkstra's algorithm.
The program simulates a road network with varying traffic conditions and road closures, and calculates the shortest path to the nearest electric vehicle (EV) charging stations.

## Features

- Graph representation of a road network
- Roads have dynamic traffic conditions (LOW, MEDIUM, HIGH) and can be closed or open.
- Random traffic condition updates.
- Dijkstra's algorithm to find the shortest path considering traffic and road closures.
- Regular updates every 5 seconds to simulate real-time traffic changes.
- Displays the shortest path, distance, EV station information, and traffic conditions from a source location.

## How It Works

1. **Graph Construction**: The graph is constructed with predefined locations and edges representing roads between them.
2. **Random Traffic Conditions**: Each edge (road) is assigned a random traffic condition.
3. **Dijkstra's Algorithm**: The algorithm calculates the shortest path from a source location to all other locations, considering road closures and traffic conditions.
4. **Real-Time Simulation**: The traffic conditions are updated every 5 seconds, and the shortest path is recalculated and displayed.

## Code Overview

- **Graph and Adjacency List Structures**: Define the graph, adjacency list, and nodes.
- **Random Traffic Condition Generation**: Generates random traffic conditions for roads.
- **Edge Addition**: Adds edges to the graph with specified weights, road closures, and traffic conditions.
- **Traffic Condition Updates**: Updates the traffic conditions of all roads in the graph.
- **Dijkstra's Algorithm**: Computes the shortest path from the source to all other vertices, considering road closures and traffic conditions.
- **Result Display**: Prints the shortest distance, path, EV station information, and traffic conditions.

## Usage

1. **Compile the Code**:
   ```sh
   gcc  final_sub.c -o final_sub
  
2. Run the Program :
   ```sh
   ./final-sub

## Dependencies
Standard C library (stdlib.h, stdio.h, stdbool.h, limits.h, time.h, unistd.h)

## Contributing
Feel free to submit issues and pull requests. Contributions are welcome!

##Author :-
Rohan Gupta
