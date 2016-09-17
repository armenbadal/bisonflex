
' ֆակտորիալ
FUNCTION Factorial(n)
  IF n = 1 THEN
    Factorial = 1
  ELSE
    Factorial = n * Factorial(n-1)
  END IF
END FUNCTION

