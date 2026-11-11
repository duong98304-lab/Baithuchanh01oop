#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>

std::string toLowerStr(const std::string& str) {
    std::string res = str;
    for (size_t i = 0; i < res.length(); ++i) {
        res[i] = (char)std::tolower((unsigned char)res[i]);
    }
    return res;
}

class Vehicle {
private:
    int vehicleID;          
    std::string vehicleType; 
    int entryTime;          
    int parkingDuration;    
    double parkingFee;     
    bool isOccupied;        

public:
    Vehicle() 
        : vehicleID(0), vehicleType(""), entryTime(0), parkingDuration(0), parkingFee(0.0), isOccupied(false) {}
        
    Vehicle(int id, std::string type, int entry, int duration)
        : vehicleID(id), vehicleType(type), entryTime(entry), parkingDuration(duration), parkingFee(0.0), isOccupied(true) {
        calculateFee();
    }
    
    int getVehicleID() const { return vehicleID; }
    std::string getVehicleType() const { return vehicleType; }
    int getEntryTime() const { return entryTime; }
    int getParkingDuration() const { return parkingDuration; }
    double getParkingFee() const { return parkingFee; }
    bool getIsOccupied() const { return isOccupied; }
    
    void setVehicleID(int id) { vehicleID = id; }
    void setVehicleType(const std::string& type) { vehicleType = type; }
    void setEntryTime(int entry) { entryTime = entry; }
    void setParkingDuration(int duration) { parkingDuration = duration; }
    void setParkingFee(double fee) { parkingFee = fee; }
    void setIsOccupied(bool occupied) { isOccupied = occupied; }
    
    void calculateFee() {
        std::string type = toLowerStr(vehicleType);
        double baseRate = 0.0;

        if (type == "car") {
            baseRate = 6.00;
        } else if (type == "motorcycle") {
            baseRate = 4.00;
        } else if (type == "bicycle") {
            baseRate = 2.00;
        } else {
            baseRate = 0.0;
        }
        
        bool isPeak = (entryTime >= 6 && entryTime <= 9) || (entryTime >= 15 && entryTime <= 18);

        int first2Hours = (parkingDuration > 2) ? 2 : parkingDuration;
        int remainingHours = (parkingDuration > 2) ? (parkingDuration - 2) : 0;
        double first2Rate = isPeak ? (baseRate * 1.30) : (baseRate * 0.95);
        double remainingRate = baseRate * 0.90;

        parkingFee = (first2Hours * first2Rate) + (remainingHours * remainingRate);
    }
    
    void display() const {
        std::cout << "  - ID xe: " << vehicleID
                  << " | Loai xe: " << vehicleType
                  << " | Gio vao: " << entryTime << "h"
                  << " | Thoi luong: " << parkingDuration << "h"
                  << " | Phi do: $" << std::fixed << std::setprecision(2) << parkingFee << std::endl;
    }
};

class ParkingLot {
private:
    int parkID;  
    char size;      
    Vehicle vehicle1;
    Vehicle vehicle2;
    Vehicle vehicle3;
    bool hasVehicle1;
    bool hasVehicle2;
    bool hasVehicle3;

public:
    ParkingLot() 
        : parkID(0), size('S'), hasVehicle1(false), hasVehicle2(false), hasVehicle3(false) {}

    ParkingLot(int id, char s)
        : parkID(id), size((char)std::toupper((unsigned char)s)), hasVehicle1(false), hasVehicle2(false), hasVehicle3(false) {}
        
    int getParkID() const { return parkID; }
    char getSize() const { return size; }
    bool getHasVehicle1() const { return hasVehicle1; }
    bool getHasVehicle2() const { return hasVehicle2; }
    bool getHasVehicle3() const { return hasVehicle3; }
    Vehicle getVehicle1() const { return vehicle1; }
    Vehicle getVehicle2() const { return vehicle2; }
    Vehicle getVehicle3() const { return vehicle3; }
    
    void setParkID(int id) { parkID = id; }
    void setSize(char s) { size = (char)std::toupper((unsigned char)s); }
    void setHasVehicle1(bool status) { hasVehicle1 = status; }
    void setHasVehicle2(bool status) { hasVehicle2 = status; }
    void setHasVehicle3(bool status) { hasVehicle3 = status; }
    void setVehicle1(const Vehicle& v) { vehicle1 = v; }
    void setVehicle2(const Vehicle& v) { vehicle2 = v; }
    void setVehicle3(const Vehicle& v) { vehicle3 = v; }
    
    int getCapacity() const {
        char upperSize = (char)std::toupper((unsigned char)size);
        if (upperSize == 'S') return 1;
        if (upperSize == 'M') return 2;
        if (upperSize == 'L') return 3;
        return 0;
    }
    
    int getVehicleCount() const {
        int count = 0;
        if (hasVehicle1) count++;
        if (hasVehicle2) count++;
        if (hasVehicle3) count++;
        return count;
    }
    
    bool isFull() const {
        return getVehicleCount() >= getCapacity();
    }
    
    bool containsVehicle(int vID) const {
        if (hasVehicle1 && vehicle1.getVehicleID() == vID) return true;
        if (hasVehicle2 && vehicle2.getVehicleID() == vID) return true;
        if (hasVehicle3 && vehicle3.getVehicleID() == vID) return true;
        return false;
    }
    
    bool addVehicle(const Vehicle& v) {
        if (isFull()) return false;
        if (containsVehicle(v.getVehicleID())) return false;

        int cap = getCapacity();
        if (cap >= 1 && !hasVehicle1) {
            vehicle1 = v;
            hasVehicle1 = true;
            return true;
        }
        if (cap >= 2 && !hasVehicle2) {
            vehicle2 = v;
            hasVehicle2 = true;
            return true;
        }
        if (cap >= 3 && !hasVehicle3) {
            vehicle3 = v;
            hasVehicle3 = true;
            return true;
        }
        return false;
    }
    
    bool removeVehicle(int vID) {
        if (hasVehicle1 && vehicle1.getVehicleID() == vID) {
            hasVehicle1 = false;
            vehicle1 = Vehicle();
            return true;
        }
        if (hasVehicle2 && vehicle2.getVehicleID() == vID) {
            hasVehicle2 = false;
            vehicle2 = Vehicle();
            return true;
        }
        if (hasVehicle3 && vehicle3.getVehicleID() == vID) {
            hasVehicle3 = false;
            vehicle3 = Vehicle();
            return true;
        }
        return false;
    }
    
    void displayLotInfo() const {
        std::cout << "=== BAI DO XE ID: " << parkID << " (Kich thuoc: " << size 
                  << " | So xe: " << getVehicleCount() << "/" << getCapacity() << ") ===" << std::endl;
        
        if (getVehicleCount() == 0) {
            std::cout << "  (Bai do hien dang trong)" << std::endl;
            return;
        }

        if (hasVehicle1) vehicle1.display();
        if (hasVehicle2) vehicle2.display();
        if (hasVehicle3) vehicle3.display();
    }
};

class SystemInterface {
private:
    ParkingLot parkingLot1;
    ParkingLot parkingLot2;
    ParkingLot parkingLot3;
    bool hasLot1;
    bool hasLot2;
    bool hasLot3;

    bool vehicleExistsInSystem(int vID) const {
        if (hasLot1 && parkingLot1.containsVehicle(vID)) return true;
        if (hasLot2 && parkingLot2.containsVehicle(vID)) return true;
        if (hasLot3 && parkingLot3.containsVehicle(vID)) return true;
        return false;
    }

    void createParkingLot() {
        if (hasLot1 && hasLot2 && hasLot3) {
            std::cout << "Loi: He thong da dat gioi han toi da 3 bai do xe!" << std::endl;
            return;
        }

        int id;
        char size;
        std::cout << "Nhap ID bai do xe: ";
        std::cin >> id;

        if ((hasLot1 && parkingLot1.getParkID() == id) ||
            (hasLot2 && parkingLot2.getParkID() == id) ||
            (hasLot3 && parkingLot3.getParkID() == id)) {
            std::cout << "Loi: ID bai do " << id << " da ton tai!" << std::endl;
            return;
        }

        std::cout << "Nhap kich thuoc bai do (S/M/L): ";
        std::cin >> size;
        size = (char)std::toupper((unsigned char)size);
        if (size != 'S' && size != 'M' && size != 'L') {
            std::cout << "Loi: Kich thuoc khong hop le! (Chi chap nhan S, M hoac L)" << std::endl;
            return;
        }

        if (!hasLot1) {
            parkingLot1 = ParkingLot(id, size);
            hasLot1 = true;
            std::cout << "-> Tao thanh cong Bai do ID " << id << " (Kich thuoc " << size << ")!" << std::endl;
        } else if (!hasLot2) {
            parkingLot2 = ParkingLot(id, size);
            hasLot2 = true;
            std::cout << "-> Tao thanh cong Bai do ID " << id << " (Kich thuoc " << size << ")!" << std::endl;
        } else if (!hasLot3) {
            parkingLot3 = ParkingLot(id, size);
            hasLot3 = true;
            std::cout << "-> Tao thanh cong Bai do ID " << id << " (Kich thuoc " << size << ")!" << std::endl;
        }
    }

    void addVehicleToSystem() {
        if (!hasLot1 && !hasLot2 && !hasLot3) {
            std::cout << "Loi: Chua co bai do xe nao trong he thong!" << std::endl;
            return;
        }

        int vID, entryTime, duration;
        std::string vType;

        std::cout << "Nhap ID xe: ";
        std::cin >> vID;

        if (vehicleExistsInSystem(vID)) {
            std::cout << "Loi: Ma xe " << vID << " da ton tai trong he thong!" << std::endl;
            return;
        }

        std::cout << "Nhap loai xe (Car/Motorcycle/Bicycle): ";
        std::cin >> vType;
        std::string typeLower = toLowerStr(vType);
        if (typeLower != "car" && typeLower != "motorcycle" && typeLower != "bicycle") {
            std::cout << "Loi: Loai xe khong hop le! (Chi chap nhan Car, Motorcycle hoac Bicycle)" << std::endl;
            return;
        }

        std::cout << "Nhap gio vao bai (0 - 23): ";
        std::cin >> entryTime;
        if (entryTime < 0 || entryTime > 23) {
            std::cout << "Loi: Gio vao bai phai tu 0 den 23!" << std::endl;
            return;
        }

        std::cout << "Nhap thoi luong do du kien (1 - 23 gio): ";
        std::cin >> duration;
        if (duration < 1 || duration > 23) {
            std::cout << "Loi: Thoi luong do phai tu 1 den 23 gio!" << std::endl;
            return;
        }

        Vehicle v(vID, vType, entryTime, duration);
        if (hasLot1 && !parkingLot1.isFull()) {
            parkingLot1.addVehicle(v);
            std::cout << "-> Da them thanh cong xe ID " << vID << " vao Bai do ID " << parkingLot1.getParkID() << std::endl;
            return;
        }
        if (hasLot2 && !parkingLot2.isFull()) {
            parkingLot2.addVehicle(v);
            std::cout << "-> Da them thanh cong xe ID " << vID << " vao Bai do ID " << parkingLot2.getParkID() << std::endl;
            return;
        }
        if (hasLot3 && !parkingLot3.isFull()) {
            parkingLot3.addVehicle(v);
            std::cout << "-> Da them thanh cong xe ID " << vID << " vao Bai do ID " << parkingLot3.getParkID() << std::endl;
            return;
        }

        std::cout << "Loi: Tat ca cac bai do xe hien co deu o day!" << std::endl;
    }

    void removeParkingLot() {
        int id;
        std::cout << "Nhap ID bai do xe can xoa: ";
        std::cin >> id;

        if (hasLot1 && parkingLot1.getParkID() == id) {
            hasLot1 = false;
            parkingLot1 = ParkingLot();
            std::cout << "-> Da xoa Bai ID " << id << " ra khoi he thong." << std::endl;
            return;
        }
        if (hasLot2 && parkingLot2.getParkID() == id) {
            hasLot2 = false;
            parkingLot2 = ParkingLot();
            std::cout << "-> Da xoa bai ID " << id << " ra khoi he thong." << std::endl;
            return;
        }
        if (hasLot3 && parkingLot3.getParkID() == id) {
            hasLot3 = false;
            parkingLot3 = ParkingLot();
            std::cout << "-> Da xoa bai ID " << id << " ra khoi he thong." << std::endl;
            return;
        }

        std::cout << "Loi: Khong tim thay bai do xe nao co ID " << id << "!" << std::endl;
    }

    void removeVehicleFromSystem() {
        if (!hasLot1 && !hasLot2 && !hasLot3) {
            std::cout << "Loi: Chua co bai do xe nao trong he thong!" << std::endl;
            return;
        }

        int vID;
        std::cout << "Nhap ID xe can lay ra: ";
        std::cin >> vID;

        if (hasLot1 && parkingLot1.containsVehicle(vID)) {
            parkingLot1.removeVehicle(vID);
            std::cout << "-> Da lay xe ID " << vID << " ra khoi Bai do ID " << parkingLot1.getParkID() << std::endl;
            return;
        }
        if (hasLot2 && parkingLot2.containsVehicle(vID)) {
            parkingLot2.removeVehicle(vID);
            std::cout << "-> Da lay xe ID " << vID << " ra khoi Bai do ID " << parkingLot2.getParkID() << std::endl;
            return;
        }
        if (hasLot3 && parkingLot3.containsVehicle(vID)) {
            parkingLot3.removeVehicle(vID);
            std::cout << "-> Da lay xe ID " << vID << " ra khoi Bai do ID " << parkingLot3.getParkID() << std::endl;
            return;
        }

        std::cout << "Loi: Khong tim thay xe co ID " << vID << " trong he thong!" << std::endl;
    }

    void displayAllLots() const {
        if (!hasLot1 && !hasLot2 && !hasLot3) {
            std::cout << "Loi: Chua co bai do xe nao trong he thong!" << std::endl;
            return;
        }
        if (hasLot1) parkingLot1.displayLotInfo();
        if (hasLot2) parkingLot2.displayLotInfo();
        if (hasLot3) parkingLot3.displayLotInfo();
    }

public: 
    SystemInterface() 
        : hasLot1(false), hasLot2(false), hasLot3(false) {}

    void runMenu() {
        int choice = 0;
        do {
            std::cout << "\n================ HE THONG QUAN LY BAI DO XE ================\n";
            std::cout << "1. Tao bai do xe moi\n";
            std::cout << "2. Them xe vao bai do\n";
            std::cout << "3. Xoa mot bai xe\n";
            std::cout << "4. Lay xe ra khoi bai do\n";
            std::cout << "5. Hien thi danh sach bai do va xe\n";
            std::cout << "6. Thoat chuong trinh\n";
            std::cout << "Lua chon cua ban (1-6): ";
            std::cin >> choice;

            switch (choice) {
                case 1: createParkingLot(); break;
                case 2: addVehicleToSystem(); break;
                case 3: removeParkingLot(); break;
                case 4: removeVehicleFromSystem(); break;
                case 5: displayAllLots(); break;
                case 6: std::cout << "Cam on ban da tin dung chuong trinh!\n"; break;
                default: std::cout << "Vui long chon lai (1-6)!\n"; break;
            }
        } while (choice != 6);
    }
};

int main() {
    SystemInterface sys;
    sys.runMenu();
    return 0;
}
