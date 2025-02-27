# 📡 Static Single Point Positioning (SPP) using GNSS Data

This repository contains an **implementation of Static Single Point Positioning (SPP) using GPS pseudoranges**.  
It is part of **Lab 1 - ENGO 465** at the **University of Calgary**, focusing on **least-squares estimation** for positioning using GNSS data.

---

## **📂 Data Files Provided**
### ✅ **1. `satpos.txt`** - Satellite Positions and Pseudorange Corrections
- Contains **satellite coordinates (`xᵢˢ, yᵢˢ, zᵢˢ`)** and **pseudorange corrections**.
- Used to **correct the measured pseudorange** before applying least-squares estimation.

### ✅ **2. `*.22o` (RINEX Observation File)**
- Logs **pseudorange measurements (`Pᵢ`)** from a standalone GPS receiver.
- Used to extract **observation epochs** and corresponding **satellite PRNs**.

### ✅ **3. `pillar_true_pos.xlsx`** - True Receiver Position (Reference)
- Contains **ground-truth receiver position** for accuracy assessment.
- Used to **compute position errors (RMS, DOP, etc.)**.

---

## **📜 Lab Overview**
This implementation follows the **7-step methodology** to compute a **GNSS position solution** using **Least Squares Estimation**:

1️⃣ **Read the RINEX observation file** (`*.22o`) to extract **pseudorange measurements (`Pᵢ`)**.  
2️⃣ **Read the satellite position file** (`satpos.txt`) to extract **satellite positions (`x, y, z`)** and **pseudorange corrections**.  
3️⃣ **Initialize the receiver state** with an approximate position and clock bias.  
4️⃣ **Compute the design matrix (`A`) and misclosure vector (`w`)**.  
5️⃣ **Solve for position corrections using Least Squares (`d𝑟̂ = -(AᵀPA)⁻¹AᵀPw`)**.  
6️⃣ **Check for convergence (`|d𝑟̂| < 10⁻⁵`)**, iterating if necessary.  
7️⃣ **Output final receiver position (`X, Y, Z, clock bias`)** and compare with **true position for accuracy assessment**.

---

<!-- ## **⚙️ How to Run the Code**
### **🔹 1️⃣ Install Dependencies**
Ensure you have the required dependencies installed:
```sh
sudo apt install libeigen3-dev  # Install Eigen library (for Linux) -->
