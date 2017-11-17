@ECHO OFF

echo "" > test.out
for /L %%i in (1,1,100) do (
make_input.py
my < input.txt > tmy.txt
pasha < input.txt > other_answer.txt
type input.txt >> test.out
type other_answer.txt >> test.out
fc other_answer.txt tmy.txt >> test.out )