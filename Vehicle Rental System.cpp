#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class Car {
private:
    string carId;
    string brand;
    string model;
    double basePricePerDay;
    bool isAvailable;

public:
    Car(string carId, string brand, string model, double basePricePerDay)
        : carId(carId), brand(brand), model(model), basePricePerDay(basePricePerDay), isAvailable(true) {}

    string getCarId() const {
        return carId;
    }

    string getBrand() const {
        return brand;
    }

    string getModel() const {
        return model;
    }

    double calculatePrice(int rentalDays) const {
        return basePricePerDay * rentalDays;
    }

    bool available() const {
        return isAvailable;
    }

    void rent() {
        isAvailable = false;
    }

    void returnCar() {
        isAvailable = true;
    }
};

class Customer {
private:
    string customerId;
    string name;

public:
    Customer(string customerId, string name)
        : customerId(customerId), name(name) {}

    string getCustomerId() const {
        return customerId;
    }

    string getName() const {
        return name;
    }
};

class Rental {
private:
    Car* car;
    Customer* customer;
    int days;

public:
    Rental(Car* car, Customer* customer, int days)
        : car(car), customer(customer), days(days) {}

    Car* getCar() const {
        return car;
    }

    Customer* getCustomer() const {
        return customer;
    }

    int getDays() const {
        return days;
    }
};

class CarRentalSystem {
private:
    vector<Car> cars;
    vector<Customer> customers;
    vector<Rental> rentals;

    Customer* getCustomerByName(const string& name) {
        for (auto& customer : customers) {
            if (customer.getName() == name) {
                return &customer;
            }
        }
        return nullptr;
    }

    Car* getCarById(const string& carId) {
        for (auto& car : cars) {
            if (car.getCarId() == carId) {
                return &car;
            }
        }
        return nullptr;
    }

public:
    void addCar(const Car& car) {
        cars.push_back(car);
    }

    void addCustomer(const Customer& customer) {
        customers.push_back(customer);
    }

    void rentCar(Car* car, Customer* customer, int days) {
        if (car->available()) {
            car->rent();
            rentals.push_back(Rental(car, customer, days));
            cout << "Car rented successfully!\n";
        } else {
            cout << "Car is not available for rent.\n";
        }
    }

    void returnCar(Car* car) {
        car->returnCar();
        Rental* rentalToRemove = nullptr;
        for (auto it = rentals.begin(); it != rentals.end(); ++it) {
            if (it->getCar() == car) {
                rentalToRemove = &(*it);
                rentals.erase(it);
                break;
            }
        }
        if (rentalToRemove != nullptr) {
            cout << "Car returned successfully!\n";
        } else {
            cout << "Car was not rented.\n";
        }
    }

    void menu() {
        int choice;
        string customerName, carId;
        int rentalDays;

        while (true) {
            cout << "\n===== Car Rental System =====\n";
            cout << "1. Rent a Car\n";
            cout << "2. Return a Car\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // To clear buffer

            if (choice == 1) {
                cout << "\n== Rent a Car ==\n";
                cout << "Enter your name: ";
                getline(cin, customerName);

                // Check if customer already exists
                Customer* existingCustomer = getCustomerByName(customerName);
                if (existingCustomer == nullptr) {
                    // Create new customer if not found
                    string customerId = "CUS" + to_string(customers.size() + 1);
                    Customer newCustomer(customerId, customerName);
                    addCustomer(newCustomer);
                    existingCustomer = &customers.back();
                }

                // Show available cars
                cout << "\nAvailable Cars:\n";
                for (const auto& car : cars) {
                    if (car.available()) {
                        cout << car.getCarId() << " - " << car.getBrand() << " " << car.getModel() << endl;
                    }
                }

                cout << "\nEnter the car ID you want to rent: ";
                getline(cin, carId);

                cout << "Enter the number of days for rental: ";
                cin >> rentalDays;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume any leftover input

                Car* selectedCar = getCarById(carId);
                if (selectedCar != nullptr && selectedCar->available()) {
                    double totalPrice = selectedCar->calculatePrice(rentalDays);
                    cout << "\n== Rental Information ==\n";
                    cout << "Customer ID: " << existingCustomer->getCustomerId() << endl;
                    cout << "Customer Name: " << existingCustomer->getName() << endl;
                    cout << "Car: " << selectedCar->getBrand() << " " << selectedCar->getModel() << endl;
                    cout << "Rental Days: " << rentalDays << endl;
                    cout << "Total Price: $" << totalPrice << endl;

                    cout << "\nConfirm rental (Y/N): ";
                    char confirm;
                    cin >> confirm;
                    if (confirm == 'Y' || confirm == 'y') {
                        rentCar(selectedCar, existingCustomer, rentalDays);
                    } else {
                        cout << "\nRental canceled.\n";
                    }
                } else {
                    cout << "\nInvalid car selection or car not available.\n";
                }
            } 
            else if (choice == 2) {
                cout << "\n== Return a Car ==\n";
                cout << "Enter the car ID you want to return: ";
                getline(cin, carId);

                Car* carToReturn = getCarById(carId);
                if (carToReturn != nullptr && !carToReturn->available()) {
                    returnCar(carToReturn);
                } else {
                    cout << "Invalid car ID or car was not rented.\n";
                }
            } 
            else if (choice == 3) {
                cout << "\nThank you for using the Car Rental System!\n";
                break;
            } 
            else {
                cout << "Invalid choice. Please enter a valid option.\n";
            }
        }
    }
};

int main() {
    CarRentalSystem rentalSystem;

    Car car1("C001", "Toyota", "Camry", 60.0);
    Car car2("C002", "Honda", "Accord", 70.0);
    Car car3("C003", "Mahindra", "Thar", 150.0);
    rentalSystem.addCar(car1);
    rentalSystem.addCar(car2);
    rentalSystem.addCar(car3);

    rentalSystem.menu();

    return 0;
}
