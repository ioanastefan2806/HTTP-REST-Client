# 📚 HTTP REST Client for Library Access 📚  
**Copyright 2023 - 2024 Stefan Ioana 322CA**  

## 🚀 About the Application  
This application is an **HTTP REST client** that allows users to interact with a **library system**.  
Users can:  
✅ **Register** an account  
✅ **Log in** to their account  
✅ **Enter** the library  
✅ **View** all books in the library  
✅ **Get details** of a specific book  
✅ **Add** a new book  
✅ **Delete** a book  

## 🛠️ Compilation  
To compile the application, use the following command:  
```sh
make
```

## ▶️ Running the Application  
To run the application, execute:  
```sh
./client
```

## ⚙️ Implementation Details  
The application follows these steps:  
1️⃣ The application starts and initializes the connection data (an array of routes, structures for cookies, and tokens).  
2️⃣ It reads the **user command** from the keyboard.  
3️⃣ It **validates** the command.  
4️⃣ It checks the **command type**, which can be:  
      - `register`  
      - `login`  
      - `enter_library`  
      - `get_books`  
      - `get_book`  
      - `add_book`  
      - `delete_book`  
      - `logout`  
      - `exit`  
5️⃣ To **modify** the library, the user must **enter the library**.  
6️⃣ To **enter the library**, the user must **be logged in**.  
7️⃣ To **log in**, the user must have a **registered account**.  
8️⃣ The user can view all books, get details of a book by its **ID**, add a new book, or delete a book.  

### 📂 File Structure  
- `requests.cpp` → Handles **POST, GET, DELETE** request creation.  
- `helpers.cpp` → Manages **server connection** and **data transmission**.  
- `client.cpp` → Contains `main()` that reads user commands and executes functions accordingly.  

### 📄 Data Format  
- Book & authentication data are **sent and received as JSON objects**.  
- **JSON parsing** is done using the **nlohmann/json.hpp** library.  
- This library is chosen because it is **header-only**, making integration easy and efficient.  

## 🖥️ How to Use  
This application is used to interact with the **library system**. Users can **register**, **log in**, **browse books**, and **manage books** (add/delete).  

### 📝 Commands  

#### 🆕 Register  
```sh
register  
username=<username>  
password=<password>  
```

#### 🔑 Login  
```sh
login  
username=<username>  
password=<password>  
```

#### 📖 Enter Library  
```sh
enter_library  
```

#### 📚 View All Books  
```sh
get_books  
```

#### 🔍 Get Book Details  
```sh
get_book  
id=<id>  
```

#### ➕ Add a Book  
```sh
add_book  
title=<title>  
author=<author>  
genre=<genre>  
publisher=<publisher>  
page_count=<page_count>  
```

#### ❌ Delete a Book  
```sh
delete_book  
id=<id>  
```

#### 🚪 Logout  
```sh
logout  
```

#### ❎ Exit  
```sh
exit  
```

## 🎯 Final Thoughts  
This project **helped me understand** the structure of an HTTP **client-server** system through a **REST API**. 💡  
It gave me insights into **how HTTP requests work** at a **low level** and how an **HTTP client** functions. 🔥  

💻 **Happy coding & enjoy your library adventure!** 📚🚀
