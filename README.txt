# USC EE450 Socket Programming Project - Fall 2022

## Overview
This project for EE450 (Fall 2022) involves implementing a simple web registration system at USC using socket programming. It simulates a scenario where students use a client application to interact with a central web registration server, which further communicates with department and credential servers.

## Components
- **Client**: Simulates student interaction with the registration system.
- **Main Server (ServerM)**: Coordinates with backend servers and manages requests.
- **Credential Server (ServerC)**: Verifies student identities.
- **Department Servers (ServerCS and ServerEE)**: Hold course information for respective departments.
## System Architecture Overview

### Client
**Description:** 
- Communicates only with serverM for authentication and details retrieval.
- Uses TCP connection.
- Allows three attempts for username and password authentication.
- Continuously prompts for CourseCode and Category post-authentication.

**Message Format:**
- **To serverM:** 
  - UserName, Password, CourseCode, Category (as pointer arrays).
- **From serverM:** 
  - Authentication Response (charArray): 2 (valid), 1 (username correct, password incorrect), 0 (username incorrect).
  - Course Response (charArray): Checks first 2 elements; 'NF' indicates course not found.

### serverM
**Description:**
- Coordinates between client and server C/EE/CS.
- Manages both TCP (with client) and UDP (with other servers) connections.

**Message Format:**
- **With Client:**
  - Receives username, password, CourseCode, Category (charArray).
  - Sends authentication and course responses (charArray).
- **With serverC:**
  - Sends encoded username and password (charArray).
  - Receives authentication response (charArray).
- **With serverEE/CS:**
  - Sends CourseCode, Category (charArray).
  - Receives final response (charArray).

### serverC
**Description:**
- Manages user authentication via UDP communication with serverM.
- Validates credentials against stored data.

**Message Format:**
- **With serverM:**
  - Receives encoded credentials (charArray).
  - Sends authentication response (charArray).

### serverEE and serverCS
**Description:**
- Process course queries from serverM via UDP.
- Store and search course details.

**Message Format:**
- **With serverM:**
  - Receives CourseCode and Category (charArray).
  - Sends final response or 'NF' for not found (pointer to array).

## Assumptions and Reused Code
- Assumptions include correct category inputs and no abrupt client closure after successful authentication.
- Reused code includes TCP connection setup from Beej's guide and UDP connection code from GeeksForGeeks, with appropriate credits in the code comments.
