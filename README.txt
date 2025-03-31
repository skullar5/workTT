steps to compile code: 

to compile code locally:
use CMakeLists.txt and VisualStudio to run code

Note:
.cpp, .h, csv files are in work/work/test
data subfolder contains all json files to save and load the data


First install visual studio: https://visualstudio.microsoft.com/downloads/ 

Now install qt: https://www.qt.io/download-qt-installer 

1. After you run the installer, log in with a qt account
2. Choose custom installation 
    a. Then choose browse and navigate into your C:\Qt folder and check if you have a Qt_Msvc folder and if not then create it
        :\i. i.e., I had to create the Qt_Msvc folder since I didnt have it and navigated to C:\Qt_Mscv
    b. Select the Qt_Mscv folder
    c. Then click on the top down for qt on the Select Component section
    d. Expand on the Qt 6.8.2 option (or the version you have on your computer)
    e. Then click option MSVC 2022 64-bit 
    f. Further expand on Additional Libraries > Select Qt Charts
    g. Then install

Then, 

open visual studio installer:

1. Then in workloads
    a. select Desktop development with C++
    b. click on install
2. Open visual studio > extensions > manage extensions 
    a. search for Qt visual studio tools
    b. install it
    c. restart visual studio when prompted
3. After reopening, on the very top click on select Qt version
4. The options window will pop up
    a. Click on add
        i. Enter a name for the Qt version i.e., Qt6.8.2_MSCV2019_64
        ii. Click the ... button next to the Location field
        iii. Then browse to your Qt installation folder.
                i.e., If you installed Qt 6.8.2 with MSVC 2019 64-bit, the path should be C:\Qt\6.8.2\Qt_Msvc or in my case its C:\Qt_Msvc
        iv. Select Qt_Mscv folder 
        v. Then navigate to the msvc2022_64 folder 
        vi. Copy the path and paste in Location field, i.e, for me its C:\Qt_Msvc\6.8.2\msvc2022_64
        vii. select your host (windows, mac, etc)
5. Then press ok
6. Now open visual studio
    a. Go to File > Open > CMake (but before doing this make sure your have the zip downloaded and extracted)
        i. You want to navigate to your test folder (i.e., work\work\test)
    b. Select CMakeLists.txt and click Open
    c. Now all the files should be loaded in the Solution Explorer on the right
        i. Scroll down and find CMakesLists.txt
        ii. Hover over to CMakeLists.txt on the Solution Explorer
        iii. Right Click > Click Delete Cache and Reconfigure
    d. Then at the top you will find a green run icon which will say Current Document (CMakeLists.txt)
    e. Click on the drop down icon beside the "Current Document (CMakeLists.txt)" and click on the NoomyPlan.exe

Remember everytime you want to run the program you must do the following (these steps must be done everytime):
1. Right click on CMakeLists.txt in the Solution Explorer and click on the "Delete Cache and Reconfigure"
2. Wait for the "CMake generation finished." message in the output then 
3. Then click on the dropdown beside the green run icon and select NoomyPlan.exe

THESE STEPS MUST BE DONE EVERY TIME PROGRAM NEEDS TO BE RAN. 


If you gett a intellisense error or "Edit and Continue" error, relaunch the ide and do steps 1 to 3 again. 

DISCLAIMER: If the option of "delete cache and reconfigure" is gray then click on the continue button at the top beside the green run icon. If you get exceptions when you do this, you have to keep on clicking
on the continue button and then it will let you proceed. 


-------------------------------------------------------- IGNORE THESE STEPS -----------------------------------------------------------------------------------------
Note: 
- Also install all the Qt visual tool extensions in visual studio
- When you create the new project remember to choose the cmake option 

When using CMakeLists.txt 

1. right click and click on the Delete Cache and Reconfigure
2. Then click on the dropdown menu beside the "Select Startup Item" which is at the very top 
3. click NoomyPlan.exe
4. click the run icon and it will run

Steps 1 to 4 should work for both windows and mac since these steps are done within Visual Studio


===================================================================================================================================================================
THESE STEPS WERE FOR GAUL BUT SINCE WE ARE USING QCHART, IT WONT COMPILE IN GAUL SO PLEASE IGNORE THESE STEPS SINCE AND USE ABOVE STEPS TO COMPILE LOCALLY
===================================================================================================================================================================

The following steps will work on putty (and if using VcXsrv server to run code using X server)

To run on putty:
1. Install VcXsrv
2. Open putty and enable X11 by doing SSH (on left menu) > + symbol beside enable X11 forwarding 
3. host name: <uwo_id>@cs3307.gaul.csd.uwo.ca 
4. click Open
5. navigate to folder with code for example: cd 3307/stage3
6. complete following steps

step 1:

find /usr -name "moc"


step 2a: 

rm moc_budgetpage.cpp moc_budgetpageexpenses.cpp

step 2b:

/usr/lib/qt6/libexec/moc budgetpagebudget.h -o moc_budgetpagebudget.cpp
/usr/lib/qt6/libexec/moc budgetpageexpenses.h -o moc_budgetpageexpenses.cpp


step 3: 

g++ -std=c++17 -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui -fPIC \
    -c budgetpagebudget.cpp -o budgetpagebudget.o


step 4:

g++ -std=c++17 -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui -fPIC \
    -c moc_budgetpagebudget.cpp -o moc_budgetpagebudget.o


step 5:

    step 5a:

    g++ -std=c++17 -c main.cpp -o main.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5b: 

    g++ -std=c++17 -c user.cpp -o user.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5c:

    g++ -std=c++17 -c authenticatesystem.cpp -o authenticatesystem.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5d:

    g++ -std=c++17 -c loginpage.cpp -o loginpage.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5e:

    g++ -std=c++17 -c financialreport.cpp -o financialreport.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5f:

    g++ -std=c++17 -c cashflowtracking.cpp -o cashflowtracking.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5g:

    g++ -std=c++17 -c budgetpageexpenses.cpp -o budgetpageexpenses.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5h:

    g++ -std=c++17 -c budgetpagebudget.cpp -o budgetpagebudget.o
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui 
    -fPIC 
    

    step 5j:

    g++ -std=c++17 -c moc_budgetpagebudget.cpp -o moc_budgetpagebudget.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5k:

    g++ -std=c++17 -c moc_budgetpageexpenses.cpp -o moc_budgetpageexpenses.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5l:

    g++ -std=c++17 -o NoomyPlan \
    main.o user.o authenticatesystem.o loginpage.o financialreport.o cashflowtracking.o budgetpagebudget.o \
    budgetpage.o budgetpageexpenses.o moc_budgetpage.o moc_budgetpageexpenses.o \
    -L/usr/lib/x86_64-linux-gnu \
    -lQt6Widgets -lQt6Core -lQt6Gui -lQt6Network -lQt6Sql -fPIC

   


step 6:

./NoomyPlan

if you get error for above open XLaunch then do step 5k again and then step 6 again and it should run

Note: moc files need to be regenerated since they can be outdated, causing errors
