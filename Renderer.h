#pragma once
#include <chrono>
#include <iostream>
#include <array>

static auto start = std::chrono::high_resolution_clock::now();
static auto stop = std::chrono::high_resolution_clock::now();
static auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);


#define Time(x) \
 start = std::chrono::high_resolution_clock::now();\
 x;\
 stop = std::chrono::high_resolution_clock::now();\
 duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);\
 std::cout << #x << " took: " << duration.count() << " microseconds" << std::endl\

#define TimeBlock(x, name) \
 start = std::chrono::high_resolution_clock::now();\
 x;\
 stop = std::chrono::high_resolution_clock::now();\
 duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);\
 std::cout << name << " took: " << duration.count() << " microseconds"<< std::endl\
