#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Reservation {
    int seatNumber;
    char passengerName[50];
    struct Reservation* left;
    struct Reservation* right;
};

struct Reservation* createReservation(int seatNumber, const char* passengerName) {
    struct Reservation* newReservation = (struct Reservation*)malloc(sizeof(struct Reservation));
    newReservation->seatNumber = seatNumber;
    strcpy(newReservation->passengerName, passengerName);
    newReservation->left = NULL;
    newReservation->right = NULL;
    return newReservation;
}

struct Reservation* insertReservation(struct Reservation* root, int seatNumber, const char* passengerName) {
    if (root == NULL) {
        return createReservation(seatNumber, passengerName);
    }

    if (seatNumber < root->seatNumber) {
        root->left = insertReservation(root->left, seatNumber, passengerName);
    } else if (seatNumber > root->seatNumber) {
        root->right = insertReservation(root->right, seatNumber, passengerName);
    }

    return root;
}

struct Reservation* searchReservation(struct Reservation* root, int seatNumber) {
    if (root == NULL || root->seatNumber == seatNumber) {
        return root;
    }

    if (seatNumber < root->seatNumber) {
        return searchReservation(root->left, seatNumber);
    }

    return searchReservation(root->right, seatNumber);
}

// Binary search tree in-order traversal
void inOrderTraversal(struct Reservation* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("Seat Number: %d, Passenger Name: %s\n", root->seatNumber, root->passengerName);
        inOrderTraversal(root->right);
    }
}

void saveReservationToFile(FILE* file, struct Reservation* root) {
    if (root != NULL) {
        saveReservationToFile(file, root->left);
        fprintf(file, "%d %s\n", root->seatNumber, root->passengerName);
        saveReservationToFile(file, root->right);
    }
}

struct User {
    char username[50];
    char password[50];
    struct User* next;
};

// Create users
struct User* createUser(const char* username, const char* password) {
    struct User* newUser = (struct User*)malloc(sizeof(struct User));
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->next = NULL;
    return newUser;
}

void loadUsers(struct User** users) {
    FILE* file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Error opening user data file.\n");
        return;
    }

    char username[50];
    char password[50];

    while (fscanf(file, "%s %s", username, password) == 2) {
        struct User* newUser = createUser(username, password);
        newUser->next = *users;
        *users = newUser;
    }

    fclose(file);
}

int authenticateUser(struct User* users, const char* username, const char* password) {
    struct User* currentUser = users;
    while (currentUser != NULL) {
        if (strcmp(currentUser->username, username) == 0 && strcmp(currentUser->password, password) == 0) {
            if (strcmp(currentUser->username, "admin") == 0) {
                return 2; // Admin authentication successful
            }
            return 1; // Authentication successful
        }
        currentUser = currentUser->next;
    }
    return 0; // Authentication failed
}

struct User* registerUser(struct User* users) {
    char username[50];
    char password[50];

    printf("Enter a new username: ");
    scanf("%s", username);

    // Check if the username already exists
    struct User* currentUser = users;
    while (currentUser != NULL) {
        if (strcmp(currentUser->username, username) == 0) {
            printf("Username already exists. Please choose a different one.\n");
            return users;
        }
        currentUser = currentUser->next;
    }

    printf("Enter a password: ");
    scanf("%s", password);

    struct User* newUser = createUser(username, password);
    newUser->next = users;
    printf("Registration successful!\n");

    return newUser;
}

// Cancel reservation
struct Reservation* cancelReservation(struct Reservation* root, int seatNumber) {
    if (root == NULL) {
        return NULL; // Reservation not found
    }

    if (seatNumber < root->seatNumber) {
        root->left = cancelReservation(root->left, seatNumber);
    } else if (seatNumber > root->seatNumber) {
        root->right = cancelReservation(root->right, seatNumber);
    } else {
        // Found the reservation to cancel
        // Case 1: No child or only one child
        if (root->left == NULL) {
            struct Reservation* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Reservation* temp = root->left;
            free(root);
            return temp;
        }

        // Case 2: Node with two children, get the inorder successor (smallest in the right subtree)
        struct Reservation* minValueNode = root->right;
        while (minValueNode->left != NULL) {
            minValueNode = minValueNode->left;
        }

        // Copy the inorder successor's content to this node
        root->seatNumber = minValueNode->seatNumber;
        strcpy(root->passengerName, minValueNode->passengerName);

        // Delete the inorder successor
        root->right = cancelReservation(root->right, minValueNode->seatNumber);
    }
    return root;
}

struct Train {
    int trainNumber;
    char trainName[50];
    char source[50];
    char destination[50];
    char departureTime[10]; // Assuming a fixed format (e.g., "HH:MM")
    int totalSeats;
    int availableSeats;
    struct Train* next;
};

struct Train* createTrain(int trainNumber, const char* trainName, const char* source, const char* destination, const char* departureTime, int totalSeats) {
    struct Train* newTrain = (struct Train*)malloc(sizeof(struct Train));
    newTrain->trainNumber = trainNumber;
    strcpy(newTrain->trainName, trainName);
    strcpy(newTrain->source, source);
    strcpy(newTrain->destination, destination);
    strcpy(newTrain->departureTime, departureTime);
    newTrain->totalSeats = totalSeats;
    newTrain->availableSeats = totalSeats; // Initially, all seats are available
    newTrain->next = NULL;
    return newTrain;
}

// Function to insert a new train into the list
struct Train* insertTrain(struct Train* head, int trainNumber, const char* trainName, const char* source, const char* destination, const char* departureTime, int totalSeats) {
    struct Train* newTrain = createTrain(trainNumber, trainName, source, destination, departureTime, totalSeats);
    newTrain->next = head;
    return newTrain;
}

void displayTrainDetails(struct Train* head) {
    struct Train* currentTrain = head;
    while (currentTrain != NULL) {
        printf("Train Number: %d\n", currentTrain->trainNumber);
        printf("Train Name: %s\n", currentTrain->trainName);
        printf("Source: %s\n", currentTrain->source);
        printf("Destination: %s\n", currentTrain->destination);
        printf("Departure Time: %s\n", currentTrain->departureTime);
        printf("Total Seats: %d\n", currentTrain->totalSeats);
        printf("Available Seats: %d\n", currentTrain->availableSeats);
        printf("\n");
        currentTrain = currentTrain->next;
    }
}

void manageTrains(struct Train** trains) {
    int choice;
    int trainNumber, totalSeats;
                char trainName[50], source[50], destination[50], departureTime[10];

    while (1) {
        printf("\n\t\t\t    MANAGE TRAINS\n");
        printf("\n\t\t\t========================\n");
        printf("\t\t\t=   TRAIN MANAGEMENT   =\n");
        printf("\t\t\t========================\n");
        printf("\n");
        printf("              ******************************************\n");
        printf("              ||     TRAIN MANAGEMENT MENU    ||\n");
        printf("              ||----------------------------------||\n");
        printf("              ||     [1.] ADD NEW TRAIN         ||\n");
        printf("              ||     [2.] DISPLAY TRAIN DETAILS  ||\n");
        printf("              ||     [3.] BACK                   ||\n");
        printf("              ****************************************\n\n");
        printf("\tENTER YOUR CHOICE: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Add new train

                printf("Enter Train Number: ");
                scanf("%d", &trainNumber);
                printf("Enter Train Name: ");
                scanf("%s", trainName);
                printf("Enter Source: ");
                scanf("%s", source);
                printf("Enter Destination: ");
                scanf("%s", destination);
                printf("Enter Departure Time (HH:MM): ");
                scanf("%s", departureTime);
                printf("Enter Total Seats: ");
                scanf("%d", &totalSeats);

                *trains = insertTrain(*trains, trainNumber, trainName, source, destination, departureTime, totalSeats);
                printf("Train added successfully!\n");
                break;

            case 2:
                // Display train details
                displayTrainDetails(*trains);
                break;

            case 3:
                // Return to the main menu
                return;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}

int main() {
    struct Reservation* root = NULL;
    int choice, seat;
    char name[50];
    char username[50], password[50];
    struct User* users = NULL;
    const char* filename = "users.txt";
    struct Train* trains = NULL;
    FILE* file = fopen("reservations.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%d %s", &seat, name) != EOF) {
            root = insertReservation(root, seat, name);
        }
        fclose(file);
    }

    while (1) {
        time_t currentTime;
        time(&currentTime);
        printf("\n\t\t\t     %s\n", ctime(&currentTime));
        printf("\n\t\t\t=================================\n");
        printf("\t\t\t=   RAILWAY MANAGEMENT SYSTEM   =\n");
        printf("\t\t\t=================================\n");
        printf("\n\n");
        printf("\n\n");
        printf("              ******************************************\n");
        printf("              ||      CHOOSE YOUR LIST     ||\n");
        printf("              ||--------------------------------------||\n");
        printf("              ||        [1.] LOGIN                    ||\n");
        printf("              ||        [2.] SIGN UP                  ||\n");
        printf("              ||        [3.] BOOK A SEAT              ||\n");
        printf("              ||        [4.] SEARCH FOR A RESERVATION ||\n");
        printf("              ||        [5.] CANCEL RESERVATIONS      ||\n");
        printf("              ||        [6.] DISPLAY ALL RESERVATIONS ||\n");
        printf("              ||        [7.] MANAGE TRAINS            ||\n");
        printf("              ||        [8.] ADMIN LOGIN              ||\n");
        printf("              ||        [9.] EXIT                     ||\n");
        printf("              ****************************************\n\n");

        printf("\n\tENTER YOUR CHOICE: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter your username: ");
                scanf("%s", username);
                printf("Enter your password: ");
                scanf("%s", password);
                int authResult = authenticateUser(users, username, password);
                if (authResult == 1) {
                    printf("Authentication successful!\n");
                    // Proceed with user-specific options
                } else if (authResult == 0) {
                    printf("Authentication failed. Invalid username or password.\n");
                }
                break;

            case 2:
                // Sign up
                users = registerUser(users);
                break;

            case 3:
                printf("Enter seat number: ");
                scanf("%d", &seat);
                printf("Enter passenger name: ");
                scanf("%s", name);
                root = insertReservation(root, seat, name);
                break;

            case 4:
                printf("Enter seat number to search: ");
                scanf("%d", &seat);
                struct Reservation* foundReservation = searchReservation(root, seat);
                if (foundReservation != NULL) {
                    printf("Reservation found: Seat Number: %d, Passenger Name: %s\n",
                           foundReservation->seatNumber, foundReservation->passengerName);
                } else {
                    printf("Reservation not found for seat number %d\n", seat);
                }
                break;

            case 5:
                // Cancel reservation
                printf("Enter seat number to cancel reservation: ");
                scanf("%d", &seat);
                root = cancelReservation(root, seat);
                if (root != NULL) {
                    printf("Reservation canceled for seat number %d.\n", seat);
                } else {
                    printf("Reservation not found for seat number %d.\n", seat);
                }
                break;

            case 6:
                printf("All reservations:\n");
                inOrderTraversal(root);
                break;

            case 7:
                // Manage Trains
                manageTrains(&trains);
                break;

            case 8:
                // Admin login
                printf("Enter admin username: ");
                scanf("%s", username);
                printf("Enter admin password: ");
                scanf("%s", password);

                int adminAuthResult = authenticateUser(users, username, password);
                if (adminAuthResult == 2) {
                    printf("Admin authentication successful!\n");
                    // Proceed with admin options (e.g., data management)
                } else {
                    printf("Admin authentication failed. Invalid username or password.\n");
                }
                break;

            case 9:
                // Save reservations to a file before exiting
                file = fopen("reservations.txt", "w");
                if (file != NULL) {
                    saveReservationToFile(file, root);
                    fclose(file);
                }
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
    return 0;
}