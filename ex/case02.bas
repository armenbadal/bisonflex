
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

