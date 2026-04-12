# 📊 Comparative Analysis of Arrays and Singly Linked Lists  
## Urban Carbon Emission Data System (CT077-3-2-DSTR Lab Work #1)

---

## 📌 Project Overview
This project is developed for the **Data Structures module (CT077-3-2-DSTR)**.  
It simulates and analyzes urban transportation data from three cities to study carbon emissions using **Arrays** and **Singly Linked Lists**.

The system compares the performance of both data structures in handling:
- Sorting
- Searching
- Data analysis
- Carbon emission calculations

---

## 🏙️ Dataset Description
The system processes data from three datasets:

- City A (Metropolitan City) → Cars, buses, bicycles, walking  
- City B (University Town) → Students using bicycles, walking, school buses, carpools  
- City C (Suburban/Rural Area) → Cars, buses, bicycles  

Each resident record contains:
- Resident ID
- Age
- Mode of Transport
- Daily Distance (km)
- Carbon Emission Factor (kg CO₂/km)
- Average Days per Month

---

## 🧠 Features Implemented

### 📂 Data Structures
- Array-based implementation
- Singly Linked List implementation

---

### 🔍 Searching Operations
- Search by Age Group  
- Search by Mode of Transport  
- Search by Daily Distance threshold  

---

### 🔃 Sorting Operations
Implemented sorting by:
- Age  
- Daily Distance  
- Carbon Emission  

Sorting algorithm used: Bubble Sort / Selection Sort (custom implementation)

---

### 🌱 Carbon Emission Analysis
- Total emissions per dataset  
- Emissions per transport mode  
- Average emissions per resident  
- Age group-based emission analysis  

---

### 👥 Age Group Classification
- 6–17 → Children & Teenagers  
- 18–25 → University Students / Young Adults  
- 26–45 → Working Adults (Early Career)  
- 46–60 → Working Adults (Late Career)  
- 61–100 → Senior Citizens / Retirees  

---

## ⚖️ Performance Comparison
This project compares:
- Time complexity of Arrays vs Linked Lists  
- Memory usage differences  
- Efficiency of sorting and searching operations  

---

## 🛠️ Technologies Used
- C++ Programming Language  
- Custom Array Implementation  
- Custom Singly Linked List Implementation  
- Console-based Text Output  

---

## 🚀 How to Run

### Compile
```bash
g++ main.cpp -o program
```

### Run (Linux / Mac)
```bash
./program
```

### Run (Windows)
```bash
program.exe
```

---

## 📁 Project Structure
```
Project Folder/
│
├── main.cpp
├── Array.hpp / Array.cpp
├── LinkedList.hpp / LinkedList.cpp
├── dataset files (.csv / .txt)
└── README.md
```

---

## 📊 Learning Outcomes
- Understanding Arrays vs Linked Lists  
- Implementation of Sorting & Searching Algorithms  
- Real-world data structure application (carbon emissions)  
- Performance analysis of data structures  

---

## ⚠️ Notes
- No STL (vector, list, etc.) was used  
- All data structures are custom implemented  
- Project is console-based only (no GUI)  

---

## 👨‍💻 Authors
CT077-3-2-DSTR Group Project  
Sirin Aktas
Ahmed Seddiq
