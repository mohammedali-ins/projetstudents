#include <iostream>
#include <memory>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;

const string DB_URL = "tcp://127.0.0.1:3306";
const string DB_USER = "root";      // Replace with your username
const string DB_PASS = "password";  // Replace with your password
const string DB_NAME = "etudiant_db";

void addStudent(sql::Connection* con) {
    string nom, prenom, date_naissance;
    cout << "Nom: "; cin >> nom;
    cout << "Prenom: "; cin >> prenom;
    cout << "Date de naissance (YYYY-MM-DD): "; cin >> date_naissance;

    unique_ptr<sql::PreparedStatement> stmt(
        con->prepareStatement("INSERT INTO etudiant(nom, prenom, date_naissance) VALUES (?, ?, ?)")
    );
    stmt->setString(1, nom);
    stmt->setString(2, prenom);
    stmt->setString(3, date_naissance);
    stmt->executeUpdate();
    cout << " Etudiant ajouté.\n";
}

void viewStudents(sql::Connection* con) {
    unique_ptr<sql::Statement> stmt(con->createStatement());
    unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM etudiant"));
    cout << "\n Liste des étudiants:\n";
    while (res->next()) {
        cout << res->getInt("id") << ". "
             << res->getString("nom") << " "
             << res->getString("prenom") << " - "
             << res->getString("date_naissance") << endl;
    }
}

void modifyStudent(sql::Connection* con) {
    int id;
    string nom;
    cout << "ID de l'étudiant à modifier: "; cin >> id;
    cout << "Nouveau nom: "; cin >> nom;

    unique_ptr<sql::PreparedStatement> stmt(
        con->prepareStatement("UPDATE etudiant SET nom = ? WHERE id = ?")
    );
    stmt->setString(1, nom);
    stmt->setInt(2, id);
    stmt->executeUpdate();
    cout << " Nom modifié.\n";
}

void deleteStudent(sql::Connection* con) {
    int id;
    cout << "ID de l'étudiant à supprimer: "; cin >> id;

    unique_ptr<sql::PreparedStatement> stmt(
        con->prepareStatement("DELETE FROM etudiant WHERE id = ?")
    );
    stmt->setInt(1, id);
    stmt->executeUpdate();
    cout << " Étudiant supprimé.\n";
}

int main() {
    try {
        sql::Driver* driver = get_driver_instance();
        unique_ptr<sql::Connection> con(driver->connect(DB_URL, DB_USER, DB_PASS));
        con->setSchema(DB_NAME);

        int choice;
        do {
            cout << "\n=== MENU ===\n";
            cout << "1. Ajouter un étudiant\n";
            cout << "2. Voir tous les étudiants\n";
            cout << "3. Modifier un étudiant\n";
            cout << "4. Supprimer un étudiant\n";
            cout << "0. Quitter\n";
            cout << "Choix: ";
            cin >> choice;

            switch (choice) {
                case 1: addStudent(con.get()); break;
                case 2: viewStudents(con.get()); break;
                case 3: modifyStudent(con.get()); break;
                case 4: deleteStudent(con.get()); break;
                case 0: cout << " Au revoir !\n"; break;
                default: cout << " Choix invalide.\n";
            }
        } while (choice != 0);

    } catch (sql::SQLException& e) {
        cerr << "Erreur SQL: " << e.what() << endl;
    }

    return 0;
}
