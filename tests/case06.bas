' case06.bas
' պայմանի կամ ճյուղավորմն հրաման

FUNCTION Main()
  x = 77
  y = 0
  
  ' պարզ դեպք
  IF x > y THEN
    PRINT x
  END IF

  ' մեկ այլընտրանք
  IF x <> y THEN
    PRINT y
  ELSE
    PRINT x
  END IF

  ' շատ այլընտրանքներ
  IF x = y THEN
    PRINT x + y
  ELSEIF x < y THEN
    PRINT x - y
  ELSEIF x > y THEN
    PRINT x * y
  END IF

  ' լրիվ տեսք
  IF x * y <> 0 THEN
    PRINT y + 1
  ELSEIF x / y < 0 THEN
    PRINT x + 1
  ELSEIF x + y > 0 THEN
    PRINT y + 2
  ELSEIF x - y = 0 THEN
    PRINT x^y
  ELSE
    PRINT y^2
  END IF
END FUNCTION

