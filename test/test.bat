rem mfinder.exe -h
rem test with 2000 symbols and 1 MLS 5-th power see Check2000MLS1P5.txt
mfinder.exe -p 5 -i .\input\test2000MLS1P5.txt -o .\output\Result2000MLS1P5.txt
rem test with 10000 symbols and 25 MLS 5-th power see Check10000MLS25P5.txt
mfinder.exe -p 5 -i .\input\test10000MLS25P5.txt -o .\output\Result10000MLS25P5.txt
rem test with 2000 symbols and 1 MLS 9-th power see Check2000MLS1P9.txt
mfinder.exe -p 9 -i .\input\test2000MLS1P9.txt -o .\output\Result2000MLS1P9.txt
rem test with 10000 symbols and 5 MLS 9-th power see Check10000MLS5P9.txt
mfinder.exe -p 9 -i .\input\test10000MLS5P9.txt -o .\output\Result10000MLS5P9.txt
pause