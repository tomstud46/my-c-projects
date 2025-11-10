//*******************************ų********************************
//                   HEADER FILE USED IN PROJECT
//****************************************************************

#include<iostream>
#include<fstream>
#include<iomanip>
#include<stdlib.h>
using namespace std;


//***************************************************************
//                   CLASS USED IN PROJECT
//****************************************************************

class student
{
	int rollno;
	char name[50];
	int m_marks, e_marks, td_marks, cse_marks,l_marks, cs_marks;
	double per;
	char grade;
	void calculate();	//function to calculate grade
public:
	void getdata();		//function to accept data from user
	void showdata() const;	//function to show data on screen
	void show_tabular() const;
	int retrollno() const;
}; //class ends here

//***************************************************************
//    	INTRODUCTION FUNCTION
//****************************************************************

void intro()
{

	cout<<"\n\n\n\t\t\t\t    STUDENT";
	cout<<"\n\n\t\t\tCOURSE RESULT MANEGMENT SYSTEM";
	cout<<"\n\n\t\t\t\t    PROJECT\n";
	cout<<"\n\t\tSCHOOL : ADAMA SCIENCE AND TECHNOLOGY UNIVERSITY(ASTU)";
	cout<<"\n\n\n\t\t\t\t\t GROUP MEMBERS";
	cout<<"\n\n\t\t\t ->GEDION GEBRE MADORO\t\tUGR/26317/14";
	cout<<"\n\n\t\t\t ->GEZAHEGN TADESSE G/MARIAM  \tUGR/25771/14";
	cout<<"\n\n\t\t\t ->JABIR MOHAMMED KEDIR\t\tUGR/25490/14 ";
	cout<<"\n\n\t\t\t ->KHALID ALEMAR RGAt\t\tUGR/25626/14";
	cout<<"\n\n\t\t\t ->LELO MOHAMMED AHMED\t\tUGR/25557/14\n";
	cout<<"\n\t\tPLEASE ENTER ANY KEY... ";

	cin.get();
}


void student::calculate()
{
	per=(m_marks+e_marks+td_marks+cse_marks+l_marks+cs_marks)/6.0;
	if(per>=85)
		grade='A';
	else if(per>=70)
		grade='B';
	else if(per>=60)
		grade='C';
	else
		grade='F';
}

void student::getdata()
{

	cout<<"\nEnter The UGR/ID number of student ";
	cin>>rollno;
	cout<<"\n\nEnter The Name of student ";
	cin.ignore();
	cin.getline(name,50);
	cout<<"\nEnter The marks in Maths out of 100 : ";
	cin>>m_marks;
	cout<<"\nEnter The marks in English out of 100 : ";
	cin>>e_marks;
	cout<<"\nEnter The marks in Engineering Drawing out of 100 : ";
	cin>>td_marks;
	cout<<"\nEnter The marks in Fundamental programing c++ out of 100 : ";
	cin>>cse_marks;
	cout<<"\nEnter The marks in Logic out of 100 : ";
	cin>>l_marks;
	cout<<"\nEnter The marks in Emerging Technology out of 100 : ";
	cin>>cs_marks;
	calculate();
}

void student::showdata() const
{

	cout<<"\nUGR/ID number of student : "<<rollno;
	cout<<"\nName of student : "<<name;
	cout<<"\nMarks in Maths : "<<m_marks;
	cout<<"\nMarks in English : "<<e_marks;
	cout<<"\nMarks in Engineering Drawing : "<<td_marks;
	cout<<"\nMarks in Fundamental Programing c++ : "<<cse_marks;
	cout<<"\nMarks in Logic :"<<l_marks;
	cout<<"\nMarks in Emerging Technology :"<<cs_marks;
	cout<<"\nPercentage of student is  :"<<per;
	cout<<"\nGrade of student is :"<<grade;
}

void student::show_tabular() const
{
	cout<<rollno<<setw(6)<<" "<<name<<setw(10)<<m_marks<<setw(5)<<e_marks<<setw(4)<<td_marks<<setw(4)
		<<cse_marks<<setw(4)<<l_marks<<setw(4)<<cs_marks<<setw(8)<<per<<setw(6)<<grade<<endl;
}

int  student::retrollno() const
{
	return rollno;
}


//***************************************************************
//    	function declaration
//****************************************************************

void write_student();	//write the result in binary file
void display_all();	//read all results from binary file
void display_sp(int);	//accept UGR/ID and read record from binary file
void modify_student(int);	//accept UGR/ID and update result of binary file
void delete_student(int);	//accept UGR/ID and delete selected results from binary file
void class_result();	//display all results in tabular format from binary file
void result();		//display result menu
void intro();		//display welcome screen
void entry_menu();	//display entry menu on screen

//    	ENTRY / EDIT MENU FUNCTION
//****************************************************************

void entry_menu()
{
	char ch;
	int num;
	system("cls");

	cout<<"\n\n\n\tTEACHER MENU";
	cout<<"\n\n\t1.CREATE STUDENT RESULT";
	cout<<"\n\n\t2.DISPLAY ALL STUDENTS RESULT";
	cout<<"\n\n\t3.SEARCH STUDENT RESULT ";
	cout<<"\n\n\t4.MODIFY STUDENT RESULT";
	cout<<"\n\n\t5.DELETE STUDENT RESULT";
	cout<<"\n\n\t6.BACK TO MAIN MENU";
	cout<<"\n\n\tPlease Enter Your Choice:~ ";
	cin>>ch;
	system("cls");
	switch(ch)
	{
	case '1':	write_student(); break;
	case '2':	display_all(); break;
	case '3':	cout<<"\n\n\tPlease Enter The UGR/ID number: "; cin>>num;
			display_sp(num); break;
	case '4':	cout<<"\n\n\tPlease Enter The UGR/ID number: "; cin>>num;
			modify_student(num);break;
	case '5':	cout<<"\n\n\tPlease Enter The UGR/ID number: "; cin>>num;
			delete_student(num);break;
	case '6':	break;
	default:	cout<<"\a"; entry_menu();
	}
}
//***************************************************************
//    	THE MAIN FUNCTION OF PROGRAM
//****************************************************************


int main()
{
	char ch;

	intro();
	do
	{
		system("cls");

		cout<<"\n\n\n\tMAIN MENU";
		cout<<"\n\n\t1. STUDENT MENU";
		cout<<"\n\n\t2. TEACHER MENU";
		cout<<"\n\n\t3. EXIT";
		cout<<"\n\n\tPlease Select Your Option :~ ";
		cin>>ch;
		switch(ch)
		{
			case '1': result();
				break;
			case '2': entry_menu();
				break;
			case '3':
				break;
			default :cout<<"\a";
		}
    }while(ch!='3');
	return 0;
}

//***************************************************************
//    	function to write in file
//****************************************************************

void write_student()
{
	student st;
	ofstream outFile;
	outFile.open("student.dat",ios::binary|ios::app);
	st.getdata();
	outFile.write(reinterpret_cast<char *> (&st), sizeof(student));
	outFile.close();
    	cout<<"\n\nStudent result Has Been Created ";
	cin.ignore();
	cin.get();
}

//***************************************************************
//    	function to read all results from file
//****************************************************************

void display_all()
{
	student st;
	ifstream inFile;
	inFile.open("student.dat",ios::binary);
	if(!inFile)
	{
	    ("color c");
		cout<<"File could not be open !! Press any Key...";
		cin.ignore();
		cin.get();
		return;
	}
	cout<<"\n\n\n\t\tDISPLAY ALL RESULTS !!!\n\n";
	while(inFile.read(reinterpret_cast<char *> (&st), sizeof(student)))
	{
		st.showdata();
		cout<<"\n\n====================================\n";
	}
	inFile.close();
	cin.ignore();
	cin.get();
}

//***************************************************************
//    	function to read specific result from file
//****************************************************************

void display_sp(int n)
{
	student st;
	ifstream inFile;
	inFile.open("student.dat",ios::binary);
	if(!inFile)
	{
	    ("color c");
		cout<<"File could not be open !! Press any Key...";
		cin.ignore();
		cin.get();
		return;
	}
	bool flag=false;
	while(inFile.read(reinterpret_cast<char *> (&st), sizeof(student)))
	{
		if(st.retrollno()==n)
		{
	  		 st.showdata();
			 flag=true;
		}
	}
	inFile.close();
	if(flag==false)
        ("color c");
		cout<<"\n\nresult not exist";
	cin.ignore();
	cin.get();
}

//***************************************************************
//    	function to modify results of file
//****************************************************************

void modify_student(int n)
{
	bool found=false;
	student st;
	fstream File;
	File.open("student.dat",ios::binary|ios::in|ios::out);
	if(!File)
	{
	    ("color c");
		cout<<"File could not be open !! Press any Key...";
		cin.ignore();
		cin.get();
		return;
	}
    	while(!File.eof() && found==false)
	{

		File.read(reinterpret_cast<char *> (&st), sizeof(student));
		if(st.retrollno()==n)
		{
			st.showdata();
			cout<<"\n\nPlease Enter The New Details of student: "<<endl;
			st.getdata();
		    	int pos=(-1)*static_cast<int>(sizeof(st));
		    	File.seekp(pos,ios::cur);
		    	File.write(reinterpret_cast<char *> (&st), sizeof(student));
		    	cout<<"\n\n\t Result Updated";
		    	found=true;
		}
	}
	File.close();
	if(found==false)
        ("color c");
		cout<<"\n\n Result Not Found ";
	cin.ignore();
	cin.get();
}

//***************************************************************
//    	function to delete result of file
//****************************************************************

void delete_student(int n)
{
	student st;
	ifstream inFile;
	inFile.open("student.dat",ios::binary);
	if(!inFile)
	{
	    ("color c");
		cout<<"File could not be open !! Press any Key...";
		cin.ignore();
		cin.get();
		return;
	}
	ofstream outFile;
	outFile.open("Temp.dat",ios::out);
	inFile.seekg(0,ios::beg);
	while(inFile.read(reinterpret_cast<char *> (&st), sizeof(student)))
	{
		if(st.retrollno()!=n)
		{
			outFile.write(reinterpret_cast<char *> (&st), sizeof(student));
		}
	}
	outFile.close();
	inFile.close();
	("color c");
	remove("student.dat");
	rename("Temp.dat","student.dat");
	cout<<"\n\n\tResult Deleted ..";
	cin.ignore();
	cin.get();
}

//***************************************************************
//    	function to display all students grade report
//****************************************************************

void class_result()
{
	student st;
	ifstream inFile;
	inFile.open("student.dat",ios::binary);
	if(!inFile)
	{
		cout<<"File could not be open !! Press any Key...";
		cin.ignore();
		cin.get();
		return;
	}
	cout<<"\n\n\t\tALL STUDENTS RESULT \n\n";
	cout<<"=============================================================\n";
	cout<<"R.No         Name     M    E   TD  CSE  L  CS  %age   Grade"<<endl;
	cout<<"=============================================================\n";
	while(inFile.read(reinterpret_cast<char *> (&st), sizeof(student)))
	{
		st.show_tabular();
	}
	cin.ignore();
	cin.get();
	inFile.close();
}

//***************************************************************
//    	function to display result menu
//****************************************************************

void result()
{
	char ch;
	int rno;
	system("cls");

	cout<<"\n\n\n\t\tSTUDENT MENU";
	cout<<"\n\n\n\t1. Class Result";
	cout<<"\n\n\t2. Student Report Card";
	cout<<"\n\n\t3. Back to Main Menu";
	cout<<"\n\n\n\tEnter Choice :~ ";
	cin>>ch;
	system("cls");
	switch(ch)
	{
	case '1' :	class_result(); break;
	case '2' :("color C");	cout<<"\n\n\tEnter UGR/ID Number Of Student: "; cin>>rno;
				display_sp(rno); break;
	case '3' :	break;
	default:	cout<<"\a";
	}
}

//***************************************************************
//    			END OF PROJECT
