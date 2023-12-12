#include <iostream>
#include <cstdlib>  // use for rand and srand function
#include <ctime>    // use for time function
using namespace std;

int main() {
    srand(time(0));
    char again;
    do {
        int number = rand() % 100 + 1;  // generate random number between 1 to 100

        int guess;
        int att = 0;
        const int maxatt = 10;

        cout << "\nWelcome to the Guess the Number game!" << endl;
        cout << "Game Rule: -" << endl;
        cout << "i)     Try to guess the number between 1 and 100" << endl;
        cout << "ii)    You will have 10 attempts to guess it right" << endl;

        do {
            cout << "\nEnter your guess: ";
            cin >> guess;

            if (guess > number) {
                cout << "Too high! Try again," << endl;
            } else if (guess < number) {
                cout << "Too low! Try again," << endl;
            } else {
                cout << "Congratulations! You guessed the number in " << att + 1 << " attempts" << endl;
                break;
            }
            att++;

            if (att == maxatt) {
                cout << "\nSorry, you've used up all your attempts" << endl;
                cout << "The correct number was: " << number << endl;
                cout << "Better luck next time!" << endl;
                break;
            }
            cout << "Attempts left: " << maxatt - att << endl;

        } while (guess != number);
        cout << "\nDo you want to play again? (y/n): ";
        cin >> again;

    } while (again == 'y' || again == 'Y');
    cout << "Thanks for playing! Goodbye!";
    return 0;
}