# 🌸 KNN Iris Classifier

**Student:** Mehmet Ali DÖNMEZ  
**Student ID:** 240101038  

## 📋 Project Overview

This project implements a **K-Nearest Neighbors (KNN)** algorithm to classify Iris flowers into three species based on their physical characteristics. The classifier uses the famous Iris dataset and predicts flower species using machine learning techniques.

## 🌺 Iris Species Classification

The program classifies flowers into three species:
- **🟢 Iris-Setosa** (Label: 0)
- **🔵 Iris-Versicolor** (Label: 1) 
- **🟣 Iris-Virginica** (Label: 2)

## 📊 Features Used

Each flower is characterized by four measurements:
- 📏 **Sepal Length** (cm)
- 📐 **Sepal Width** (cm)
- 📏 **Petal Length** (cm)
- 📐 **Petal Width** (cm)

## 🚀 How It Works

1. **📚 Training Phase**: Loads labeled training data from `train.csv`
2. **🔍 Distance Calculation**: For each test flower, calculates Euclidean distance to all training flowers
3. **👥 K-Neighbors Selection**: Finds the 5 closest neighbors (K=5)
4. **🗳️ Majority Vote**: Predicts the species based on the most common species among neighbors
5. **💾 Results**: Saves predictions to `predictions.txt`

## 🛠️ Compilation & Execution

### Compile the program:
```bash
gcc 240101038.c -o knn_classifier -lm
```

### Run the classifier:
```bash
./knn_classifier
```

## 🔧 Algorithm Parameters

- **K Value**: 5 neighbors
- **Distance Metric**: Euclidean distance
- **Classification Method**: Majority voting

## 📈 Performance Insights

The debug log provides detailed insights:
- 📊 Training dataset distribution
- 🎯 Distance calculations for sample flowers
- 👥 Neighbor selection process
- 📋 Final prediction statistics

## 🧮 Mathematical Foundation

### Distance Calculation:
```
d = √[(x₁-x₂)² + (y₁-y₂)² + (z₁-z₂)² + (w₁-w₂)²]
```
Where:
- x = Sepal Length
- y = Sepal Width  
- z = Petal Length
- w = Petal Width

## 📚 Dataset Information

- **Training Data**: 121 labeled flowers
- **Test Data**: 30 unlabeled flowers
- **Features**: 4 continuous measurements
- **Classes**: 3 Iris species

## 🎨 Key Features

- ✅ **Clean Code**: Well-documented and structured
- 📝 **Detailed Logging**: Comprehensive debug information
- 🎯 **Accurate Classification**: Robust KNN implementation
- 💾 **File I/O**: Handles CSV input and text output
- 🔧 **Error Handling**: Proper file validation

## 🚨 Requirements

- 💻 **Compiler**: GCC with math library support (`-lm`)
- 📊 **Input Files**: `train.csv` and `test.csv` in same directory
- 🗂️ **Permissions**: Write access for output files

## 📊 Expected Results

The classifier will generate:
1. **Prediction file** with species classifications
2. **Debug log** with detailed analysis

---
