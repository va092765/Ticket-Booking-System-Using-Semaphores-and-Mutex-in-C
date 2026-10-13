# Multithreaded Ticket Booking System in C

## 📌 Project Overview

This project implements a multithreaded ticket booking system using
POSIX threads (pthreads), semaphores, and mutex locks in C.

The system simulates multiple members attempting to book tickets
simultaneously.

A semaphore limits the number of members who can enter the booking
counter at the same time, while a mutex protects the shared ticket
count from race conditions.

The program demonstrates important Operating System concepts,
including thread synchronization, critical sections, and shared
resource management.

## 🎯 Objectives

- Understand multithreading in C.
- Implement POSIX threads using pthreads.
- Use semaphores to control concurrent access.
- Use mutex locks to protect shared resources.
- Prevent race conditions during ticket booking.
- Simulate a real-world ticket reservation system.

## ⚙️ Technologies Used

- Programming Language: C
- Operating System: Linux
- POSIX Threads (pthreads)
- POSIX Semaphores
- Mutex Locks
- GCC Compiler

## 🔑 Key Features

- Simulates multiple members booking tickets concurrently.
- Starts with 60 total tickets.
- Reserves 20 tickets in advance.
- Makes 40 tickets available for booking.
- Allows a maximum of 2 members inside the booking counter
  simultaneously.
- Limits each booking request to 10 tickets.
- Checks ticket availability before confirming a booking.
- Displays booking success or failure messages.
- Uses mutex locks to protect the shared ticket count.

## 🧠 Operating System Concepts

### 1. Multithreading

Each member is represented by a separate thread.

Multiple threads can execute concurrently and attempt to book tickets.

### 2. Semaphore

The semaphore controls access to the booking counter.

The maximum number of concurrent members is configured as 2.

```c
sem_t booking_sem;
