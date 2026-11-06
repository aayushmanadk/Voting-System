#include<iostream>
#include<fstream>
#include<cstring>
#include<iomanip>
#include<cstdlib>
using namespace std;
const int MAX_CANDIDATES=10;
const int MAX_VOTERS=50;
// Base class:Person class
class Person{
	
protected:
char name[50];
char password[20];

public:
 virtual void getInfo();//Input the information:(name,password)
 virtual bool login(const char *pass);//Checks the password to the stored password
 virtual void show();//Displays the person's name
 const char* getName();//Returns the name for the person
};

//Definition outside the class
void Person::getInfo(){              //Ask the user to add information regarding the person
	cout<<"Enter the name:"<<endl;
	cin>>name;
	cout<<"Enter the password:"<<endl;
	cin>>password;
	
}
bool Person::login(const char *pass){ //Returns logic based on comparsion of passwords
	return strcmp(pass,password)==0;
}

void Person::show(){                 //Displays the person name
	cout<<"Name:"<<name<<endl;
}

const char* Person::getName(){     //Returns the person name for the program section
	return name;
}
class Candidate:public Person{     //Derived candiate class inherit from Person 
int votes;

public:
Candidate():votes(0){                //Initialize attributes using constructor
name[0]='\0';
password[0]='\0';
}            
void getInfo() override{
cout<<"Enter candidate details:\n"; //Add the details of candidate
Person::getInfo();
}

void addvote(){                  //Increase candidate votes by 1
votes++;
}

int getVotes() const{           //Return candidate's votes
  return votes;
}
//File handling program to save and load the candidates data and info
void save(ofstream& out) const{    //Save the data to text file
 out<<name<<" "<<password<<" "<<votes <<endl;	
}

bool load(ifstream& in){          //Loads the data from the text file
	return static_cast<bool>(in>>name>>password>>votes);
}
};
class Voter:public Person{       //Derived Voter class from Person class
bool hasVoted;                   //hasVoted to check votes
public:
 Voter():hasVoted(false){        //Initialize hasVoted to false to prevent multiple votings
 	name[0]='\0';
 	password[0]='\0';
 }

void getInfo() override{         //Add the details of voters
	cout<<"Enter voter details:\n";
	Person::getInfo();
}
bool markVote(){                 //Marks the voters vote and check if they cast it already
	if(!hasVoted){
	 hasVoted=true;
	 return true;
	}
	return false;
}
//File handling program to save and load the voters data and info
void save(ofstream &out) const{   //Read info and voting status of voter
out<<name<<" "<<password<<" "<<(hasVoted?1:0)<<endl;
} 
bool load(ifstream &in){          //Loads the info and status from file to program
int temp;
if(in>>name>>password>>temp)
{
	hasVoted=(temp!=0);            //Restore the voting status from the file
	return true;                
}
    return false;
}
bool voted()const{               //Method to return hasVoted
	return hasVoted;
}
};

//File handling
void saveCandidates(Candidate cands[], int n) {
    ofstream out("candidates.txt");
    for (int i = 0; i < n; ++i) {
        cands[i].save(out);
    }
    out.close()  ;
}

int loadCandidates(Candidate cands[]){        //Loads the candidate list from candidates.txt file
	ifstream in("candidates.txt");
	int n=0;
	while(n<MAX_CANDIDATES && cands[n].load(in)){ //Calls load method from candidate class till n times
	n++; }                                        //Reads the next array element
	in.close();
	return n;
}
void saveVoters(Voter voters[], int n) {     //Save the voters list to voters.txt file
    ofstream out("voters.txt");
    for (int i = 0; i < n; ++i){
	
        voters[i].save(out);                 //Write the data of nth voter in file
    }
	out.close();
}

int loadVoters(Voter voters[]) {             //Loads the voter list to the program
    ifstream in("voters.txt");
    int n = 0;
    while (n < MAX_VOTERS && voters[n].load(in)){ //Read voter list upto n times calling load of Voter class
       n++;
   }
    in.close();
    return n;
}
//Main program section
int main() {
    Candidate candidates[MAX_CANDIDATES];
    Voter voters[MAX_VOTERS];
    int candCount = loadCandidates(candidates);
    int voterCount = loadVoters(voters);
	
    // Debug print loaded candidates
    cout << "Loaded " << candCount << " candidates:\n";
    for (int i = 0; i < candCount; ++i) {
        cout << "  " << i+1 << ". " << candidates[i].getName() << endl;
    }

    int choice;
    do {

        cout << "\n+---------------------------+\n";
        cout << "|    Online Voting System    |\n";
        cout << "+----+----------------------+ \n";
        cout << "| No |       Option          |\n";
        cout << "+----+----------------------+ \n";
        cout << "| 1  | Add Candidate         |\n";
        cout << "| 2  | Add Voter             |\n";
        cout << "| 3  | Vote                  |\n";
        cout << "| 4  | Show Results          |\n";
        cout << "| 5  | Exit                  |\n";
        cout << "+----+----------------------+ \n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            if (candCount >= MAX_CANDIDATES) {
                cout << "Candidate limit reached!\n";
            } else {
                // Input candidate name first for duplicate check
                char tempName[50];
            

                bool duplicate = false;
                for (int i = 0; i < candCount; ++i) {
                    if (strcmp(candidates[i].getName(), tempName) == 0) {
                        duplicate = true;
                        break;
                    }
                }

                if (duplicate) {
                    cout << "Candidate already exists! Cannot add duplicate.\n";
                } else {
                    // Now input full candidate info (including password)
                    candidates[candCount].getInfo();
                    candCount++;
                    saveCandidates(candidates, candCount);
                    cout << "Candidate added successfully!\n";
                }
            }
        }
        else if (choice == 2) {
            if (voterCount >= MAX_VOTERS) {
                cout << "Voter limit reached!\n";
            } else {
                voters[voterCount++].getInfo();
                saveVoters(voters, voterCount);
                cout << "Voter added successfully!\n";
            }
        }
        else if (choice == 3) {
            char vName[50], vPass[20];
            cout << "Enter voter name: ";
            cin >> vName;
            cout << "Enter password: ";
            cin >> vPass;

            int vIdx = -1;
            for (int i = 0; i < voterCount; ++i) {
                if (strcmp(voters[i].getName(), vName) == 0 && voters[i].login(vPass)) {
                    vIdx = i;
                    break;
                }
            }

            if (vIdx == -1) {
                cout << "Voter authentication failed!\n";
                continue;
            }

            if (voters[vIdx].voted()) {
                cout << "You have already voted.\n";
                continue;
            }

            cout << "\n+----+---------------------+\n";
            cout << "| No | Candidate Name      |\n";
            cout << "+----+---------------------+\n";
            for (int i = 0; i < candCount; ++i) {
                cout << "| " << setw(2) << i + 1 << " | " << setw(19) << left << candidates[i].getName() << "|\n";
            }
            cout << "+----+---------------------+\n";

            int cIdx;
            cout << "Enter candidate number to vote: ";
            cin >> cIdx;

            if (cIdx < 1 || cIdx > candCount) {
                cout << "Invalid candidate number.\n";
                continue;
            }

            candidates[cIdx - 1].addvote();
            voters[vIdx].markVote();
            cout << "Vote cast successfully!\n";

            saveCandidates(candidates, candCount);
            saveVoters(voters, voterCount);
        }
        else if (choice == 4) {
            cout << "\n+----------------------+------------+\n";
            cout << "| Candidate Name       | Votes        |\n";
            cout << "+----------------------+------------+\n";
            for (int i = 0; i < candCount; ++i) {
                cout << "| " << setw(20) << left << candidates[i].getName()
                     << " | " << setw(10) << right << candidates[i].getVotes() << " |\n";
            }
            cout << "+----------------------+------------+\n";
        }
        else if (choice == 5) {
            cout << "Exiting...\n";
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 5);

    return 0;
}
