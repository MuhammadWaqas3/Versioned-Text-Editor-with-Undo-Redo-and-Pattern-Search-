#!/bin/bash

# run_all_tests.sh - Automated test runner for Text Editor project
# Usage: chmod +x run_all_tests.sh && ./run_all_tests.sh

echo "======================================="
echo "  TEXT EDITOR - AUTOMATED TEST SUITE"
echo "======================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
total_tests=0
passed_tests=0
failed_tests=0

# Navigate to tests directory
cd "$(dirname "$0")"

echo -e "${BLUE}Current directory: $(pwd)${NC}"
echo ""

# Clean previous builds
echo -e "${YELLOW}Cleaning previous builds...${NC}"
rm -f test_buffer test_search test_history test_integration
echo "Done."
echo ""

# ============================================================================
# TEST 1: Buffer Tests
# ============================================================================

echo -e "${BLUE}=== COMPILING TEST 1: Buffer Tests ===${NC}"
g++ -o test_buffer test_buffer.cpp ../src/Buffer.cpp -std=c++11 -I../src

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Compilation successful${NC}"
    echo ""
    
    echo -e "${BLUE}=== RUNNING TEST 1: Buffer Tests ===${NC}"
    ./test_buffer
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ All buffer tests passed${NC}"
        ((passed_tests++))
    else
        echo -e "${RED}✗ Some buffer tests failed${NC}"
        ((failed_tests++))
    fi
else
    echo -e "${RED}✗ Compilation failed${NC}"
    ((failed_tests++))
fi

((total_tests++))
echo ""
echo "-----------------------------------"
echo ""

# ============================================================================
# TEST 2: Search Tests
# ============================================================================

echo -e "${BLUE}=== COMPILING TEST 2: Search Tests ===${NC}"
g++ -o test_search test_search.cpp ../src/AdvancedSearch.cpp ../src/Buffer.cpp -std=c++11 -I../src

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Compilation successful${NC}"
    echo ""
    
    echo -e "${BLUE}=== RUNNING TEST 2: Search Tests ===${NC}"
    ./test_search
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ All search tests passed${NC}"
        ((passed_tests++))
    else
        echo -e "${RED}✗ Some search tests failed${NC}"
        ((failed_tests++))
    fi
else
    echo -e "${RED}✗ Compilation failed${NC}"
    ((failed_tests++))
fi

((total_tests++))
echo ""
echo "-----------------------------------"
echo ""

# ============================================================================
# TEST 3: History Tests
# ============================================================================

echo -e "${BLUE}=== COMPILING TEST 3: History Tests ===${NC}"
g++ -o test_history test_history.cpp ../src/History.cpp ../src/Buffer.cpp -std=c++11 -I../src

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ Compilation successful${NC}"
    echo ""
    
    echo -e "${BLUE}=== RUNNING TEST 3: History Tests ===${NC}"
    ./test_history
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ All history tests passed${NC}"
        ((passed_tests++))
    else
        echo -e "${RED}✗ Some history tests failed${NC}"
        ((failed_tests++))
    fi
else
    echo -e "${RED}✗ Compilation failed${NC}"
    ((failed_tests++))
fi

((total_tests++))
echo ""
echo "-----------------------------------"
echo ""

# ============================================================================
# FINAL SUMMARY
# ============================================================================

echo "======================================="
echo "       FINAL TEST SUMMARY"
echo "======================================="
echo ""
echo -e "Total Test Suites: ${BLUE}$total_tests${NC}"
echo -e "Passed: ${GREEN}$passed_tests${NC}"
echo -e "Failed: ${RED}$failed_tests${NC}"
echo ""

if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}✓✓✓ ALL TESTS PASSED! ✓✓✓${NC}"
    echo ""
    echo "Your project is ready for submission!"
    exit 0
else
    echo -e "${RED}✗✗✗ SOME TESTS FAILED ✗✗✗${NC}"
    echo ""
    echo "Please fix the failing tests before submission."
    exit 1
fi