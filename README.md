# Graph-Based Friend Network System

This project implements a graph-based social network using C. Users can create vertices (representing individuals) and edges (representing friendships), and interact with the network using various commands. The program supports features like finding mutual friends, calculating shortest paths between users, suggesting friends within a certain degree, and listing popular users based on their connections.

## Features

* Add Users and Friendships: Add individual users and define friendships as undirected edges
* Display Network Structure: View a complete representation of the network and each user's connections
* Mutual Friends: Find common friends between two users
* Shortest Path: Calculate the shortest path between two users
* Friend Recommendations: Suggest friends for a user within a given degree of separation
* Popularity Ranking: List users based on their total number of connections
* List Friends: View the friends of a specific user

## How to Use

### 1. Compile the Program

```bash
gcc graph_network.c -o graph_network
```

### 2. Run the Program

```bash
./graph_network
```

### 3. Enter Input for Network Setup

#### Step 1: Add Users
You will be prompted to enter the number of users and their names. Each name should be unique.

```
Enter number of users: 3
Enter user names (one per line):
User 1: Alice
User 2: Bob
User 3: Charlie
```

#### Step 2: Add Friendships
Specify the number of friendships and enter each friendship as a pair of names.

```
Enter number of friendships: 2
Enter friendships (one pair per line, separated by space):
Friendship 1: Alice Bob
Friendship 2: Bob Charlie
```

### 4. Enter Commands

The program supports various commands to interact with the network. Commands can be entered one at a time.

## Commands

### `print_graph`
Display the entire network structure, showing each user and their direct connections.

**Usage:**
```
print_graph
```

### `mutual_friends <user1> <user2>`
List mutual friends between user1 and user2.

**Usage:**
```
mutual_friends Alice Bob
```

### `shortest_path <user1> <user2>`
Find the shortest path (friendship chain) from user1 to user2.

**Usage:**
```
shortest_path Alice Charlie
```

### `recommend_friends <user> <degree>`
Recommend friends for user within the specified degree of separation.

**Usage:**
```
recommend_friends Alice 2
```

### `most_popular`
List users ranked by the number of friends (connections) they have.

**Usage:**
```
most_popular
```

### `list_friends <user>`
Display the list of friends for a specific user.

**Usage:**
```
list_friends Bob
```

### `clear`
Clear the screen (for Unix-based systems).

**Usage:**
```
clear
```

### `exit`
Exit the program.

**Usage:**
```
exit
```

## Example

```
Enter number of users: 3
Enter user names (one per line):
User 1: Alice
User 2: Bob
User 3: Charlie
Enter number of friendships: 2
Enter friendships (one pair per line, separated by space):
Friendship 1: Alice Bob
Friendship 2: Bob Charlie

Enter command: print_graph
Graph Structure:
Alice: Bob
Bob: Alice, Charlie
Charlie: Bob

Enter command: mutual_friends Alice Charlie
Mutual friends between Alice and Charlie:
- Bob
```

## Code Structure

* **Graph Structure:** The graph is represented using an adjacency matrix and an array of vertex names
* **BFS Traversal:** Breadth-first search is used for the shortest path and friend recommendations
* **Sorting:** Popularity ranking is done using a simple bubble sort

## Additional Notes

* The program is limited to MAX_VERTICES (100) users for simplicity
* It is designed as an undirected graph, assuming friendships are mutual
* Error checking is included to ensure valid inputs