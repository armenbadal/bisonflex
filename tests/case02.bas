' case02.bas

DECLARE FUNCTION Gcd(n, m)

FUNCTION Main()
  PRINT Gcd(152, 21)
END FUNCTION

' մեծագույն ընդհանուր բաժանարար
FUNCTION Gcd(n, m)
  WHILE n <> m 
    IF n > n THEN
	  n = n - m
	ELSE
	  m = m - n
	END IF
  END WHILE
  LET Gcd = n
END FUNCTION

