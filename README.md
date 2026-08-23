# School Management System

> A School Management System designed natively in C for modern administration.

## ✨ Core Features

- **Admin Portal:** System administration, database controls & account management.
- **Teacher Portal:** Class schedules, grade submissions, attendance logging, and teacher workload tracking.
- **Student Portal:** Term grades, GPA tracking, fee status, and timetable mapping.

## 🛠️ Tech Stack & Prerequisites

| Layer           | Technology                                                                                                     | Details                               |
| :-------------- | :------------------------------------------------------------------------------------------------------------- | :------------------------------------ |
| **Language**    | ![C](https://img.shields.io/badge/C-ANSI_C99-00599C?style=for-the-badge&logo=c&logoColor=white)                | Core Logic & Memory Management        |
| **Compiler**    | ![GCC](https://img.shields.io/badge/GCC-v7.0+-brightgreen?style=for-the-badge&logo=gnu&logoColor=white)        | Native C Compilation Target           |
| **Build Tool**  | ![Make](https://img.shields.io/badge/GNU_Make-Automation-orange?style=for-the-badge&logo=make&logoColor=white) | Incremental `.o` Object Linking       |
| **Storage**     | ![Binary Storage](https://img.shields.io/badge/Storage-Atomic_Binary_I/O-blueviolet?style=for-the-badge)       | Transactional Safety & Low Overhead   |
| **Terminal UI** | ![ANSI](https://img.shields.io/badge/UI-ANSI_Colors_%26_Masking-blue?style=for-the-badge)                      | Styled CLI Layouts & Password Masking |

## 🚀 Installation & Setup

```bash
# Clone the repository
git clone https://github.com/rahman-mijanur/school-management-system.git

# Navigate to project root
cd school-management-system
```

## ⚙️ Build Automation Commands

| Command          | Action                                                            |
| :--------------- | :---------------------------------------------------------------- |
| `make build`     | Compiles modified source files and links binary to `bin/app.exe`. |
| `make run`       | Compiles project and launches the CLI application.                |
| `make clean`     | Removes compiled object files (`.o`) and executable binaries.     |
| `make deepClean` | Performs clean build reset and wipes export/backup caches.        |
