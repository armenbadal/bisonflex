
# Yacc֊ի և Lex֊ի մասին

Ես պատմում եմ ծրագրավորման լեզվի շարահյուսական վերլուծիչի իրականացման մասին։ պատմությունս հնարավորին պարզ պահելու համար ցույց կտամ, թե ինչպես, օրինակ, պարզեցված Բեյսիկ (BASIC) լեզվով գրված ծրագիրը թարգմանել Լիսպ (Lisp) լեզվով գրված ծրագրի։ Բեյսիկն ընտրել եմ իր հայտնիության ու քերականության պարզության համար։ Լիսպը (տվյալ դեպքում՝ դրա Scheme տարատեսակը) նույնպես ընտրել եմ ներկայացման պարզության համա։ Բացի այդ, Լիսպ լեզվով գրված ծրագրերում ուղղակիորեն արտացոլվում է ծրագրի հիերարխիկ (ծառաձև) կառուցվածքը։


## Բեկուսի֊Նաուրի գրառում

Քանի որ և՛ վերլուծվող Բեյսիկ լեզուն սահմանելու համար, և՛ շարահյուսական վերլուծիչը GNU Bison ֆայլում կոդավորելու համար օգտագործելու եմ Բեկուսի֊Նաուրի գրառումը (BNF ― Backus-Naur Form), լավ կլինի, որ շատ կարճ խոսեմ նաև դրա մասին։

Ի՞նչ է լեզվի քերականությունը։ Նախ՝ դրա մաթեմատիկական սահմանումը․ _`L` լեզվի `G(L)` քերականությունը `<T,N,R,S>` քառյակն է, որտեղ `T`֊ն տերմինալային սիմվոլների բազմությունն է, `N`֊ը՝ ոչ տերմինալային սիմվոլներինը, `R` քերականական կանոնների (կամ հավասարումների) բազմությունն է և `S`֊ն էլ սկզբնական սիմվոլն է։_

_Տերմինալային_ սիմվոլները լեզվի քերականության անտրոհելի, ատոմար տարրերն են։ Օրինակ, ծառայողական բառերը, թվային ու տեքստային լիտերալները, մետասիմվոլները և այլն։

_Ոչ տերմինալային_ սիմվոլները լեզվի առանձին տարրերի սահմանումներին տրված անուններն են։

_Քերականական կանոնը_ լեզվի քերականության կառուցման հիմնական միավորն է, դրանով է որոշվում լեզվական կառուցվածքի տեսքը։ Քերականական կանոնը `→` (սլաք) նիշով բաժանված է ձախ ու աջ մասերի։ Ձախ կողմում սահմանվող ոչ տերմինալային սիմվոլն է, իսկ աջում՝ սահմանումը որոշող տերմինալային և ոչ տերմինալային սիմվոլների շարքը։ Օրինակ, Բեյսիկ լեզվի _վերագրման_ հրամանը սահմանող երկու քերականական կանոններն ունեն այսպիսի տեսք․

````
Assignment  → OptionalLet IDENT '=' Expression
OptionalLet → LET | ε
````

Այստեղ գլխատառերով գրված են տերմինալային սիմվոլները՝ `IDENT` և `LET`, իսկ Pascal-Case կանոնով՝ ոչ տերմինալայինները՝ `Assignment`, `Exprsssion` և `OptionalLet`։ Առաջին կանոնն «ասում է», որ վերագրման հրամանը (`Assignment`) բաղկացած է իրար հաջորդող `LET` սիմվոլից, իդենտիֆիկատորից, `=` վերգրման նշանից և վերագրվող արտահայտությունից (`Expression`)։ Երկրորդ կանոնով սահմանվում է `LET` սիմվոլի ոչ պարտադիր լինելը՝ `OptionalLet`֊ը կամ `LET` սիմվոլն է, կամ դատարկ է՝ `ε`։  Քերականական կանոնի աջ կողմում այլընտրանքային տերբերակները (alternatives) իրարից անջատվում են `|` նիշով։

_Սկզբնական սիմվոլն_ այն ոչ տերմինալային սիմվոլն է, որից պետք է սկսել լեզվի վերլուծությունը։ ․․․

Լեզվի քերականության սահմանման այս եղանակը կոչվում է _Բեկուսի֊Նաուրի գրառում_։ 


## Լեզվի սահմանում

Այստեղ քննարկվող Բեյսիկ լեզուն ունի տվյալների միայն մեկ տիպ՝ _իրական թիվ_։ Ծառայողական բառերը գրվում են միայն գլխատառերով, իդենտիֆիկատորներում մեծատառերն ու փոքրատառերը տարբերվում են (case sensitive)։

Բեյսիկի քերականությունը ես կսահմանեմ «վերևից֊ներքև»։ Այսինքն, նախ կսահմանեմ լեզվի «խոշոր» բաղադրիչները, ապա հերթականությամբ կսահմանեմ կանոններում հանդիպող բոլոր չսահմանված ոչ տերմինալային սիմվոլները։

Բեյսիկ լեզվով գրված ծրագիրը ֆունկցիաների հաջորդականություն է․

````
Program → FunctionList
````

Ֆունկցիաների հաջորդականությունը, որ կարող է նաև դատարկ լինել, սահմանեմ ռեկուրսիվ եղանակով․

````
FunctionList → FunctionList Function
             | ε
````

Ֆունկցիա սահմանումով որոշվում է և՛ ֆունկցիայի հայտարարությունը, և՛ ֆունկցիայի սահմանումը․

````
Function → Declaration
         | Definition
````

Ֆունկցիայի հայտարարությունը սկսվում է `DECLARE` ծառայողական բառով, որին հետևում է ֆունկցիայի վերնագիրը․

````
Declaration → DECLARE FunctionHeader
````

Ֆունկցիայի սահմանումը սկսվում է վերնագրով, որին հաջորդում է հրամանների ցուցակ, և ավարտվում է `END` և `FUNCTION` ծառայողական բառերով․

````
Definition → FunctionHeader StatementList END FUNCTION
````

Ֆունկցիայի վերնագիրը սկսվում է `FUNCTION` ծառայողական բառով, որին հետևում է ֆունկցիայի անունը որոշող իդենտիֆիկատոր, ապա՝ `(` և `)` փակագծերի մեջ վերցրած պարամետրերի ցուցակ։

````
FunctionHeader → FUNCTION IDENT '(' ParameterList ')' NewLines
````

Պարամետրերի ցուցակը կամ դատարկ է, կամ էլ ստորակետով իրարից բաժանված իդենտիֆիկատորների հաջորդականություն է․

````
ParameterList → IdentifierList
              | ε
````

Իդենտիֆիկատորների ցուցակը նույնպես սահմանեմ ռեկուրսիվ եղանակով․

````
IdentifierList → IdentifierList ',' IDENT
               | IDENT
````

`NewLines` ոչ տերմինալային սիմվոլով որոշվում է նոր տողի անցման մեկ և ավելի նիշերի շարքը․

՝՝՝՝
NewLines → NewLines '\n'
         | '\n'
՝՝՝՝

Հրամանների ցուցակնը, որն էլի սահմանում եմ ռեկուրսիվ կանոնով, կարող է դատարկ լինել․

````
StatementList → StatementList Statement NewLines
              | ε
````

Բեյսիկի հրամաններն են․ ներմուծում, արտածում, վերագրում, ճյուղավորում, պարամետրով ցիկլ, նախապայմանով ցիկլ, պրոցեդուրայի կանչ։ Դրանք բոլորը սահմանում եմ որպես `Statement`։

Ներմուծման հրամանը սկսվում է `INPUT` ծառայողական բառով, որին հաջորդվում է ներմուծվող փոփոխականի իդենտիֆիկատորը

````
Statement → INPUT IDENT
````

Արտածման հրամանը սկսվում է `PRINT` բառով, որին հետևում է արտածվող արտահայտությունը․

````
Statement → PRINT Expression
````

Վերագրման հրամանն արդեն սահմանել եմ վերևում, այստեղ պարզապես կրկնեմ այն․

````
Statement → OptionalLet IDENT '=' Expression
OptionalLet → LET | ε
````

Ճյուղավորման հրամանը բոլորիս լավ հայտնի `IF` կառուցվածքն է։ Այն բաղկացած է երեք կարևոր բլոկներից, որոնցից միայն առաջինն է պարտադիր։ Առաջին և պարտադիր բլոկը սկսվում է `IF` ծառայողական բառով, որին հետևում է ճյուղավորման պայմանի արտահայտությունը, հետո՝ `THEN` ծառայողական բառը, նոր տողերի նիշեր և պայմանի ճշմարիտ լինելու դեպքում կատարվող հրամանների ցուցակ։ Երկրորդ և ոչ պարդադիր բլոկը այլընտրանքային պայմանները որոշող `ElseIfPartList` ցուցակն է, որի ամեն մի էլեմենտը սկսվում է `ELSEIF` ծառայողական բառով, ապա՝ պայմանի արտահայտությունը, `THEN` ծառայողական բառը, նոր տողի նիշեր և պայմանի ճշմարիտ լինելու դեպքում կատարվող հրամանների ցուցակ։ Երրորդ և ոչ պարտադիր բլոկը սկսվում է `ELSE` ծառայողական բառով, որին հաջորդում են նոր տողի նիշեր և հրամանների շարք։ Ճյուղավորման ամբողջ կառուցվածքն ավարտվում է `END` և `IF` ծառայողական բառերով։ 

````
Statement → IF Expression THEN NewLines StatementList ElseIfPartList ElsePart END IF
ElseIfPartList → ElseIfPartList ELSEIF Expression THEN NewLines StatementList
               | ε
ElsePart → ELSE StatementList
		 | ε
````

Պարամետրով ցիկլի հրամանը սկսվում է `FOR` ծառայողական բառով, որին հաջորդում են ցիկլի պարամետրի իդենտիֆիկատորը, `=` նիշը, պարամետրի սկզբնական արժեքը որոշող արտահայտությունը, `TO` բառը, պարամետրի արժեքի վերին սահմանի արտահայտությունը, `STEP` բառը, պարամետրի քայլը որոշող արտահայտությունը, նոր տողի նիշեր, ցիկլի մարմինը որոշող հրամանների ցուցակ։ Պարամետրով ցիկլի հրամանն ավարտվում է `END` և `FOR` բառերով։

````
Statement → FOR IDENT '=' Expression TO Expression OptionalStep NewLines StatementList END FOR
OptionalStep → STEP Expression
````

Նախապայմանով ցիկլի հրամանը սկսվում է `WHILE` ծառայողական բառով, որին հետևում են ցիկլի կրկնման պայմանի արտահայտությունը, նոր տողի նիշեր, ցիկլի մարմնի հրամանների շարք, և հրամանն ավարտվում է `END` և `WHILE` բառերով։

````
Statement → WHILE Expression NewLines StatementList END WHILE
````
