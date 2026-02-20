#!/bin/bash
echo "Running basic tests..."
./lexer test.simples > output.txt
if [ $? -ne 0 ]; then
    echo "Basic test failed!"
    exit 1
fi

echo "Checking output format..."
if ! grep -q "<KEYWORD, \"programa\", 5, 1>" output.txt; then
    echo "Output check failed!"
    exit 1
fi

echo "Running error test..."
./lexer error_test.simples > error_output.txt 2>&1
if [ $? -eq 0 ]; then
    echo "Error test failed (should have exited with error)!"
    exit 1
fi

if ! grep -q "Unexpected character '@'" error_output.txt; then
    echo "Error message check failed!"
    exit 1
fi

echo "All tests passed!"
rm output.txt error_output.txt
