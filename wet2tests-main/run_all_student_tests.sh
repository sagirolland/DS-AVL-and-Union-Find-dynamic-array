#!/bin/bash

# Step 1: Compile
echo "🔧 Compiling..."
g++ -std=c++14 -DNDEBUG -Wall -o main.out ../main25b2.cpp ../dspotify25b2.cpp 2> compile_errors.txt
if [ $? -ne 0 ]; then
  echo "❌ Compilation failed"
  cat compile_errors.txt
  exit 1
fi
echo "✅ Compilation succeeded."

# Step 2: Prepare output directory
mkdir -p ResultOutputs

# Step 3 & 4: Run tests and compare outputs + memory check
echo ""
echo "🚀 Running tests and memory checks..."
pass=0
fail=0
valgrind_pass=0
valgrind_fail=0

for infile in Inputs/*.in; do
  base=$(basename "$infile" .in)
  outfile="ResultOutputs/$base.out"
  expected="ExpectedOutputs/$base.out"

  # Run program and save output
  ./main.out < "$infile" > "$outfile"

  # Output comparison
  if diff -q "$outfile" "$expected" > /dev/null; then
    output_result="✅"
    ((pass++))
  else
    output_result="❌"
    ((fail++))
  fi

  # Run Valgrind
  valgrind --leak-check=full --show-leak-kinds=definite --error-exitcode=1 ./main.out < "$infile" > /dev/null 2> "ResultOutputs/$base.val"
  if [ $? -eq 0 ]; then
    mem_result="✅"
    ((valgrind_pass++))
  else
    mem_result="❌"
    ((valgrind_fail++))
  fi

  # Combined per-test result
  printf "%-20s Output: %s   Memory: %s\n" "$base" "$output_result" "$mem_result"
done

# Final Summary
echo ""
printf "📊 Summary Report:\n"
printf "%-20s %d\n" "✅ Tests Passed:" "$pass"
printf "%-20s %d\n" "❌ Tests Failed:" "$fail"
printf "%-20s %d\n" "✅ Memory Passed:" "$valgrind_pass"
printf "%-20s %d\n" "❌ Memory Failed:" "$valgrind_fail"
