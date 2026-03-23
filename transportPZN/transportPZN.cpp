// transportPZN.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <mysql.h>
#include <string>
#include <format>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <chrono>
class Login {
private:
    std::string _username;
    std::string _password;
    int _port;
    std::string _dbName;
public:
    Login(std::string u, std::string p, int portNo, std::string databaseName) {
        _username = u;
        _password = p;
        _port = portNo;
        _dbName = databaseName;
    };
    std::string getUser() const {
        return _username;
    }
    std::string getPswd() const {
        return _password;
    }
    int getPort() const {
        return _port;
    }
    std::string getDBname()const {
        return _dbName;
    }
};

std::string baseQuery(
    std::string_view stopName,//przystanek
    std::string_view routeID, //nr lini
    std::string_view timeMin, //minimalna godzina odjazdu z przystanku
    std::string_view timeMax, //naksymalna wartosc godziny odjazdu z przystanku
    std::string_view direction, //kierunek trasy
    std::string_view weekDay //dzien tygodnia, mala litera, jezyk angielski

) {
    
    return std::format("SELECT s.stopName,t.routeID, st.departureTime, t.tripDir "
        "FROM StopTimes st "
        "JOIN Trips t ON st.tripID = t.tripID "
        "JOIN Stops s ON st.stopID = s.stopID "
        "JOIN Calendar c ON c.serviceID = LEFT(st.tripID, 1) "
        "WHERE s.stopName LIKE '%{}%' "
        "AND t.routeID = '{}' "
        "AND st.departureTime >= '{}' "
        "AND st.departureTime <= '{}' "
        "AND st.stopHeadsign LIKE '%{}%' "
        "AND c.{} = 1 "
        "ORDER BY st.departureTime ASC; ",
        stopName, routeID, timeMin, timeMax, direction, weekDay
    );
}
//bool needsUpdate() {
//    std::string calendarQuery;
//    auto now = std::chrono::system_clock::now;
//    std::string today = std::format({ "%Y-%m-%d" });
//    
//
//}
int main()
{
   
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    MYSQL* conn = mysql_init(nullptr);
    std::string username, pswd, dbName;
    int port = 0;
  
    std::cout << "Provide username and password\n";
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "\nPassword: ";
    std::cin >> pswd;
    std::cout << "\nProvide port: ";
    std::cin >> port;
    std::cout << "\nYour database name\n";
    std::cin >> dbName;
    Login defLogin(username, pswd, port, dbName);

    if (conn == NULL) {
        std::cout << "initialisation error\nterminating ...\n\n";
        return 1;
    }
    if (mysql_real_connect(conn, "localhost", defLogin.getUser().c_str(), defLogin.getPswd().c_str(), defLogin.getDBname().c_str(), defLogin.getPort(), NULL, 0)) {
        std::cout << "connection established on port " << port << "\n";
        //mysql_close(conn);
    }
    else {
        std::cout << "Connection error:\n1.Incorrect password or username\n2.Incorrect connection port\n3.Other? Check is docker running\n";
        return 1;
    }

    std::string stopName, routeID, timeMin, timeMax, direction, weekday;
    std::cout << "Provide the following input data:\n";
    std::cout << "Stop name: "; std::getline(std::cin >>std::ws,stopName);std::cout << "\n";
    std::cout << "Line number: ";std::cin >> routeID;std::cout << "\n";
    std::cout << "Min departure hour "; std::cin >> timeMin;std::cout << "\n";
    std::cout << "Max departure hour: ";std::cin >> timeMax;std::cout << "\n";
    std::cout << "Direction: "; std::getline(std::cin >> std::ws, direction);std::cout << "\n";
    std::cout << "Weekday: ";std::cin >> weekday;std::cout << "\n";
    std::string kwarenda = baseQuery(stopName, routeID, timeMin, timeMax, direction, weekday);
    mysql_query(conn, kwarenda.c_str());
    MYSQL_RES* result = mysql_store_result(conn);
    if (result) {
       MYSQL_ROW row;
     std::cout << "\nConnections in the given time period\n";
        while (row = mysql_fetch_row(result)) {
            std::cout << "\n=======================\n";
            std::cout << "Stop: " << row[0] << "\nLine: " << row[1] << "\nDeparture: " << row[2] << "\nDirection: " << row[3];
           std::cout << "\n=======================\n";
        }
        mysql_free_result(result);
    }
  


    return 0;
}


// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
