// Name: Mehmet Ali DÖNMEZ 
// Student ID: 240101038 

#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>   
#include <math.h>     

#define MAX_FLOWERS 200 // Maximum number of flowers that can be processed
#define LOG_FILE "knn_debug.log" // Log file for debugging information

// Structure to represent an Iris flower with its features and species
typedef struct {
    float sepal_length;  // Length of sepal in cm
    float sepal_width;   // Width of sepal in cm
    float petal_length;  // Length of petal in cm
    float petal_width;   // Width of petal in cm
    int species;         // Species classification
    /* 
        0 for Iris-Setosa, 
        1 for Iris-Versicolor, 
        2 for Iris-Virginica, 
        -1 for unknown
    */
} Iris;

// Structure to store distance calculation results and associated species
typedef struct {
    float distance;  // Euclidean distance between flowers
    int species;     // Species of the compared training flower
} DistanceInfo;

// Function prototypes
// Calculate Euclidean distance between two flowers
float calculateDistance(Iris flower1, Iris flower2);  

// Comparison function for sorting distances
int compareDistance(const void* a, const void* b); 

// Determine majority species among k neighbors
int getMajorityVote(DistanceInfo* neighbors, int k);  

// Convert numeric species code to string name
const char* getSpeciesName(int species);              

int main(int argc, char* argv[]) {
    // Arrays to store training and test data
    Iris training_data[MAX_FLOWERS];
    Iris test_data[MAX_FLOWERS];
    
    int train_count = 0;  // Counter for training samples
    int test_count = 0;   // Counter for test samples
    
    // Open log file for debug information
    FILE* log_file = fopen(LOG_FILE, "w");
    if (log_file == NULL) {
        printf("Hata: %s dosyası oluşturulamadı\n", LOG_FILE); 
        return 1;
    }
    
    // Open training data file
    FILE* train_file = fopen("train.csv", "r");
    if (train_file == NULL) {
        printf("Hata: train.csv acilamadi\n"); 
        return 1;
    }
    
    char line[256];
    fgets(line, sizeof(line), train_file);
    
    // Read and parse each line of training data
    while (fgets(line, sizeof(line), train_file) && train_count < MAX_FLOWERS) {
        // Extract the 5 values (4 features + species) from each line
        sscanf(line, "%f,%f,%f,%f,%d",
               &training_data[train_count].sepal_length,
               &training_data[train_count].sepal_width,
               &training_data[train_count].petal_length,
               &training_data[train_count].petal_width,
               &training_data[train_count].species);
        // Log information about loaded flower (debugging)
        fprintf(log_file, "cicek %d yuklendi: [%.2f, %.2f, %.2f, %.2f] -> %s\n", 
               train_count + 1,
               training_data[train_count].sepal_length, 
               training_data[train_count].sepal_width,
               training_data[train_count].petal_length, 
               training_data[train_count].petal_width,
               getSpeciesName(training_data[train_count].species));
        
        train_count++;
    }      
    // Log summary of training data
    fprintf(log_file, "\ntoplam %d cicek biliniyor\n", train_count);  
    int species_counts[3] = {0};  
    for (int i = 0; i < train_count; i++) {
        species_counts[training_data[i].species]++;
    }
    fprintf(log_file, "-Iris-Setosa: %d\n", species_counts[0]);
    fprintf(log_file, "-Iris-Versicolor: %d\n", species_counts[1]);
    fprintf(log_file, "-Iris-Virginica: %d\n\n", species_counts[2]);
    
    fclose(train_file);
    
    // Open test data file
    FILE* test_file = fopen("test.csv", "r");
    if (test_file == NULL) {
        printf("Hata: test.csv\n"); 
        return 1;
    }
    
    fgets(line, sizeof(line), test_file);
    
    // Read and parse each line of test data
    while (fgets(line, sizeof(line), test_file) && test_count < MAX_FLOWERS) {
        // Extract the 4 features (no species information in test data)
        sscanf(line, "%f,%f,%f,%f",
               &test_data[test_count].sepal_length,
               &test_data[test_count].sepal_width,
               &test_data[test_count].petal_length,
               &test_data[test_count].petal_width);
        
        // Initialize species as unknown (-1)
        test_data[test_count].species = -1;
        // Log information about first 5 test flowers (for debugging)
        if (test_count < 5) {
            fprintf(log_file, "test cicegi %d yuklendi: [%.2f, %.2f, %.2f, %.2f]\n", 
                   test_count + 1,
                   test_data[test_count].sepal_length, 
                   test_data[test_count].sepal_width,
                   test_data[test_count].petal_length, 
                   test_data[test_count].petal_width);
        }
        
        test_count++;
    }
    
    fclose(test_file);  
    fprintf(log_file, "\nEğitim: %d iris çiçeği\nTest: %d iris çiçeği\n", train_count, test_count);

    // Set K value for KNN algorithm (number of neighbors to consider)
    const int K = 5;
    
    fprintf(log_file, "\nk=%d\n", K);
    
    // For each test flower, find K nearest neighbors from training set
    for (int i = 0; i < test_count; i++) {    
        DistanceInfo distances[MAX_FLOWERS];  
        
        // Calculate distance from current test flower to each training flower
        for (int j = 0; j < train_count; j++) {
            distances[j].distance = calculateDistance(test_data[i], training_data[j]);
            distances[j].species = training_data[j].species;

            // Log first 5 distance calculations for first 5 test flowers (debugging)
            if (i < 5 && j < 5) {
                fprintf(log_file, "cicek %d (%s): %.f\n", 
                       j + 1, 
                       getSpeciesName(training_data[j].species),
                       distances[j].distance);
            }
        }
        
        // Sort distances in ascending order to find K nearest neighbors
        qsort(distances, train_count, sizeof(DistanceInfo), compareDistance);
        // Log K nearest neighbors for first 5 test flowers (debugging)
        if (i < 5) {
            fprintf(log_file, "\nk=%d sonuc:\n", K);
            for (int k = 0; k < K; k++) {
                fprintf(log_file, "%d. mesafe: %.4f, %s\n", 
                       k + 1, 
                       distances[k].distance,
                       getSpeciesName(distances[k].species));
            }
        }
        
        // Determine the majority species among the K nearest neighbors
        test_data[i].species = getMajorityVote(distances, K);
        // Log vote distribution for first 5 test flowers (debugging)
        if (i < 5) {
            int counts[3] = {0};
            for (int k = 0; k < K; k++) {
                counts[distances[k].species]++;
            }
            fprintf(log_file, "tahmin: %s\n", getSpeciesName(test_data[i].species));
        }
    }
    
    // Write prediction results to output file
    FILE* output_file = fopen("predictions.txt", "w");
    if (output_file == NULL) {
        printf("Hata: predictions.txt\n");  
        return 1;
    }
    
    // Write each test flower's features and predicted species
    for (int i = 0; i < test_count; i++) {
        fprintf(output_file, "Test flower %d: %.1f, %.1f, %.1f, %.1f is predicted as %d\n",
                i + 1,
                test_data[i].sepal_length,
                test_data[i].sepal_width,
                test_data[i].petal_length,
                test_data[i].petal_width,
                test_data[i].species);
    }
    
    fclose(output_file);      
    fprintf(log_file, "\n=== sonuc ===\n");  
    int prediction_counts[3] = {0}; 
    for (int i = 0; i < test_count; i++) {
        prediction_counts[test_data[i].species]++;
    }
    fprintf(log_file, "- Iris-Setosa %d\n", prediction_counts[0]);
    fprintf(log_file, "- Iris-Versicolor %d\n", prediction_counts[1]);
    fprintf(log_file, "- Iris-Virginica %d\n", prediction_counts[2]);
    printf("Predictions are done! -> predictions.txt\n");

    // Close log file
    fclose(log_file);
    printf("Debug logs saved to %s\n", LOG_FILE);
    
    return 0;
}

float calculateDistance(Iris flower1, Iris flower2) {
    return sqrt(
        pow(flower1.sepal_length - flower2.sepal_length, 2) +
        pow(flower1.sepal_width - flower2.sepal_width, 2) +
        pow(flower1.petal_length - flower2.petal_length, 2) +
        pow(flower1.petal_width - flower2.petal_width, 2)
    );
}

int compareDistance(const void* a, const void* b) {
    DistanceInfo* dist_a = (DistanceInfo*)a;
    DistanceInfo* dist_b = (DistanceInfo*)b;
    
    if (dist_a->distance < dist_b->distance) return -1;
    if (dist_a->distance > dist_b->distance) return 1;
    return 0;
}

int getMajorityVote(DistanceInfo* neighbors, int k) {
    int counts[3] = {0};
    for (int i = 0; i < k; i++) {
        counts[neighbors[i].species]++;
    }
    
    int max_count = counts[0];
    int max_species = 0;
    
    for (int i = 1; i < 3; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
            max_species = i;
        }
    }
    
    return max_species;
}

const char* getSpeciesName(int species) {
    switch(species) {
        case 0: return "Iris-Setosa";
        case 1: return "Iris-Versicolor";
        case 2: return "Iris-Virginica";
        default: return "?";  // Unknown
    }
}