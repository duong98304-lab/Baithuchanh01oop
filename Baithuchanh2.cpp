#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime>

std::string toLowerStr(const std::string& str) {
    std::string res = str;
    for (size_t i = 0; i < res.length(); ++i) {
        res[i] = (char)std::tolower((unsigned char)res[i]);
    }
    return res;
}

void clearInputBuffer() {
    std::cin.clear();
    while (std::cin.get() != '\n');
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
    void setEntryTime(int entry) { entryTime = entry; calculateFee(); }
    void setParkingDuration(int duration) { parkingDuration = duration; calculateFee(); }
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
    static const int MAX_VEHICLES = 3;
    Vehicle vehicles[MAX_VEHICLES];
    int currentVehicleCount;

public:
    ParkingLot() 
        : parkID(0), size('S'), currentVehicleCount(0) {}

    ParkingLot(int id, char s)
        : parkID(id), size((char)std::toupper((unsigned char)s)), currentVehicleCount(0) {}
        
    int getParkID() const { return parkID; }
    char getSize() const { return size; }
    int getCurrentVehicleCount() const { return currentVehicleCount; }
    
    void setParkID(int id) { parkID = id; }
    void setSize(char s) { size = (char)std::toupper((unsigned char)s); }

    int getCapacity() const {
        char upperSize = (char)std::toupper((unsigned char)size);
        if (upperSize == 'S') return 1;
        if (upperSize == 'M') return 2;
        if (upperSize == 'L') return 3;
        return 0;
    }
    
    bool isFull() const {
        return currentVehicleCount >= getCapacity();
    }
    
    int findVehicleIndex(int vID) const {
        for (int i = 0; i < currentVehicleCount; ++i) {
            if (vehicles[i].getVehicleID() == vID) {
                return i;
            }
        }
        return -1;
    }

    bool containsVehicle(int vID) const {
        return findVehicleIndex(vID) != -1;
    }
    
    bool addVehicle(const Vehicle& v) {
        if (isFull()) return false;
        vehicles[currentVehicleCount] = v;
        currentVehicleCount++;
        return true;
    }
    
    bool removeVehicle(int vID) {
        int index = findVehicleIndex(vID);
        if (index == -1) return false;

        for (int i = index; i < currentVehicleCount - 1; ++i) {
            vehicles[i] = vehicles[i + 1];
        }
        currentVehicleCount--;
        return true;
    }
    
    const Vehicle* getVehicles() const { return vehicles; }

    void displayLotInfo() const {
        std::cout << "=== BAI DO XE ID: " << parkID << " (Kich thuoc: " << size 
                  << " | So xe: " << currentVehicleCount << "/" << getCapacity() << ") ===" << std::endl;
        
        if (currentVehicleCount == 0) {
            std::cout << "  (Bai do hien dang trong)" << std::endl;
            return;
        }

        for (int i = 0; i < currentVehicleCount; ++i) {
            vehicles[i].display();
        }
    }
};

class SystemInterface {
private:
    static const int MAX_LOTS = 10;
    ParkingLot parkingLots[MAX_LOTS];
    int currentLotCount;

    int generateRandomID() const {
        return 100 + rand() % 900;
    }

    bool isParkIDExists(int pid) const {
        for (int i = 0; i < currentLotCount; ++i) {
            if (parkingLots[i].getParkID() == pid) return true;
        }
        return false;
    }

    bool isVehicleIDExists(int vID) const {
        for (int i = 0; i < currentLotCount; ++i) {
            if (parkingLots[i].containsVehicle(vID)) return true;
        }
        return false;
    }

    void createParkingLot() {
        if (currentLotCount >= MAX_LOTS) {
            std::cout << "Loi: He thong da dat gioi han toi da 10 bai do xe!\n";
            return;
        }

        int id;
        char size;
        std::cout << "Nhap ID bai do xe: ";
        while (!(std::cin >> id)) {
            std::cout << "Loi: Nhap sai kieu du lieu! Vui long nhap lai ID (so nguyen): ";
            clearInputBuffer();
        }

        if (isParkIDExists(id)) {
            int newID = generateRandomID();
            while (isParkIDExists(newID)) { newID = generateRandomID(); }
            std::cout << "[CANH BAO] ID bai do " << id << " da ton tai! Tu dong gan ID moi: " << newID << std::endl;
            id = newID;
        }

        std::cout << "Nhap kich thuoc bai do (S/M/L): ";
        std::cin >> size;
        size = (char)std::toupper((unsigned char)size);
        while (size != 'S' && size != 'M' && size != 'L') {
            std::cout << "Loi: Kich thuoc khong hop le! Vui long nhap lai (S/M/L): ";
            clearInputBuffer();
            std::cin >> size;
            size = (char)std::toupper((unsigned char)size);
        }

        parkingLots[currentLotCount] = ParkingLot(id, size);
        currentLotCount++;
        std::cout << "-> Tao thanh cong Bai do ID " << id << " (Kich thuoc " << size << ")!\n";
    }

    void addVehicleToSystem() {
        if (currentLotCount == 0) {
            std::cout << "Loi: Chua co bai do xe nao trong he thong!\n";
            return;
        }

        int vID, entryTime, duration;
        std::string vType;

        std::cout << "Nhap ID xe: ";
        while (!(std::cin >> vID)) {
            std::cout << "Loi: Nhap sai kieu du lieu! Vui long nhap lai ID xe (so nguyen): ";
            clearInputBuffer();
        }

        if (isVehicleIDExists(vID)) {
            int newVID = generateRandomID();
            while (isVehicleIDExists(newVID)) { newVID = generateRandomID(); }
            std::cout << "[CANH BAO] Ma xe " << vID << " da ton tai! Tu dong gan ID moi: " << newVID << std::endl;
            vID = newVID;
        }

        std::cout << "Nhap loai xe (Car/Motorcycle/Bicycle): ";
        std::cin >> vType;
        std::string typeLower = toLowerStr(vType);
        while (typeLower != "car" && typeLower != "motorcycle" && typeLower != "bicycle") {
            std::cout << "Loi: Loai xe khong hop le! Vui long nhap lai (Car/Motorcycle/Bicycle): ";
            clearInputBuffer();
            std::cin >> vType;
            typeLower = toLowerStr(vType);
        }

        std::cout << "Nhap gio vao bai (0 - 23): ";
        while (!(std::cin >> entryTime) || entryTime < 0 || entryTime > 23) {
            std::cout << "Loi: Gio vao phai la so nguyen tu 0 den 23! Nhap lai: ";
            clearInputBuffer();
        }

        std::cout << "Nhap thoi luong do du kien (1 - 23 gio): ";
        while (!(std::cin >> duration) || duration < 1 || duration > 23) {
            std::cout << "Loi: Thoi luong do phai la so nguyen tu 1 den 23! Nhap lai: ";
            clearInputBuffer();
        }

        Vehicle v(vID, vType, entryTime, duration);

        for (int i = 0; i < currentLotCount; ++i) {
            if (!parkingLots[i].isFull()) {
                parkingLots[i].addVehicle(v);
                std::cout << "-> Da them xe ID " << vID << " vao Bai do ID " << parkingLots[i].getParkID() << "\n";
                return;
            }
        }

        std::cout << "Loi: Tat ca cac bai do xe hien co deu da day!\n";
    }

    void removeParkingLot() {
        if (currentLotCount == 0) {
            std::cout << "Loi: He thong chua co bai do xe nao!\n";
            return;
        }

        int id;
        std::cout << "Nhap ID bai do xe can xoa: ";
        while (!(std::cin >> id)) {
            std::cout << "Loi: Nhap sai kieu du lieu! Nhap lai ID: ";
            clearInputBuffer();
        }

        int index = -1;
        for (int i = 0; i < currentLotCount; ++i) {
            if (parkingLots[i].getParkID() == id) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            std::cout << "Loi: Khong tim thay bai do xe nao co ID " << id << "!\n";
            return;
        }

        for (int i = index; i < currentLotCount - 1; ++i) {
            parkingLots[i] = parkingLots[i + 1];
        }
        currentLotCount--;
        std::cout << "-> Da xoa Bai ID " << id << " ra khoi he thong.\n";
    }

    void removeVehicleFromSystem() {
        if (currentLotCount == 0) {
            std::cout << "Loi: Chua co bai do xe nao trong he thong!\n";
            return;
        }

        int vID;
        std::cout << "Nhap ID xe can lay ra: ";
        while (!(std::cin >> vID)) {
            std::cout << "Loi: Nhap sai kieu du lieu! Nhap lai ID xe: ";
            clearInputBuffer();
        }

        for (int i = 0; i < currentLotCount; ++i) {
            if (parkingLots[i].containsVehicle(vID)) {
                parkingLots[i].removeVehicle(vID);
                std::cout << "-> Da lay xe ID " << vID << " ra khoi Bai do ID " << parkingLots[i].getParkID() << "\n";
                return;
            }
        }

        std::cout << "Loi: Khong tim thay xe co ID " << vID << " trong he thong!\n";
    }

    void displayAllLots() const {
        if (currentLotCount == 0) {
            std::cout << "He thong hien chua co bai do xe nao!\n";
            return;
        }
        for (int i = 0; i < currentLotCount; ++i) {
            parkingLots[i].displayLotInfo();
        }
    }

    void displaySpecificLot() const {
        if (currentLotCount == 0) {
            std::cout << "He thong hien chua co bai do xe nao!\n";
            return;
        }

        int id;
        std::cout << "Nhap ID bai do xe can xem chi tiet: ";
        std::cin >> id;

        for (int i = 0; i < currentLotCount; ++i) {
            if (parkingLots[i].getParkID() == id) {
                parkingLots[i].displayLotInfo();
                return;
            }
        }
        std::cout << "Loi: Khong tim thay bai do xe co ID " << id << "!\n";
    }

    void listVehiclesByType() const {
        if (currentLotCount == 0) {
            std::cout << "He thong hien chua co bai do xe nao!\n";
            return;
        }

        std::string targetType;
        std::cout << "Nhap loai xe can thong ke (Car/Motorcycle/Bicycle): ";
        std::cin >> targetType;
        std::string targetLower = toLowerStr(targetType);

        bool foundAny = false;
        std::cout << "\n=== DANH SACH XE LOAI [" << targetType << "] TRONG HE THONG ===\n";

        for (int i = 0; i < currentLotCount; ++i) {
            const Vehicle* vList = parkingLots[i].getVehicles();
            int count = parkingLots[i].getCurrentVehicleCount();

            for (int j = 0; j < count; ++j) {
                if (toLowerStr(vList[j].getVehicleType()) == targetLower) {
                    std::cout << "[Bai ID " << parkingLots[i].getParkID() << "]";
                    vList[j].display();
                    foundAny = true;
                }
            }
        }

        if (!foundAny) {
            std::cout << "  (Khong tim thay xe nao thuoc loai " << targetType << ")\n";
        }
    }

    void summarizeAverageFees() const {
        if (currentLotCount == 0) {
            std::cout << "He thong hien chua co bai do xe nao!\n";
            return;
        }

        double totalFeeCar = 0, totalFeeMoto = 0, totalFeeBike = 0;
        int countCar = 0, countMoto = 0, countBike = 0;

        std::cout << "\n=== THONG KE PHI DO XE TRUNG BINH PHAN CHIA THEO BAI ===\n";
        for (int i = 0; i < currentLotCount; ++i) {
            double lotTotalFee = 0;
            const Vehicle* vList = parkingLots[i].getVehicles();
            int count = parkingLots[i].getCurrentVehicleCount();

            for (int j = 0; j < count; ++j) {
                double fee = vList[j].getParkingFee();
                lotTotalFee += fee;

                std::string type = toLowerStr(vList[j].getVehicleType());
                if (type == "car") { totalFeeCar += fee; countCar++; }
                else if (type == "motorcycle") { totalFeeMoto += fee; countMoto++; }
                else if (type == "bicycle") { totalFeeBike += fee; countBike++; }
            }

            double lotAvg = (count > 0) ? (lotTotalFee / count) : 0.0;
            std::cout << "- Bai ID " << parkingLots[i].getParkID() 
                      << " | Tong phi: $" << std::fixed << std::setprecision(2) << lotTotalFee
                      << " | Trung binh/xe: $" << lotAvg << std::endl;
        }

        std::cout << "\n=== TRUNG BINH PHI TOAN HE THONG THEO LOAI XE ===\n";
        std::cout << "- Car: " << (countCar > 0 ? totalFeeCar / countCar : 0.0) << " $/xe (" << countCar << " xe)\n";
        std::cout << "- Motorcycle: " << (countMoto > 0 ? totalFeeMoto / countMoto : 0.0) << " $/xe (" << countMoto << " xe)\n";
        std::cout << "- Bicycle: " << (countBike > 0 ? totalFeeBike / countBike : 0.0) << " $/xe (" << countBike << " xe)\n";
    }

public: 
    SystemInterface() 
        : currentLotCount(0) {
        srand((unsigned int)time(0));
    }

    void runMenu() {
        int choice = 0;
        do {
            std::cout << "\n================ HE THONG QUAN LY BAI DO XE (GIAI DOAN 2) ================\n";
            std::cout << "1. Tao bai do xe moi\n";
            std::cout << "2. Them xe vao bai do\n";
            std::cout << "3. Xoa mot bai do xe\n";
            std::cout << "4. Lay xe ra khoi bai do\n";
            std::cout << "5. Hien thi danh sach tat ca cac Bai do xe\n";
            std::cout << "6. Hien thi danh sach xe theo bai cu the\n";
            std::cout << "7. Liet ke xe theo loai phuong tien\n";
            std::cout << "8. Tom tat phi do xe trung binh\n";
            std::cout << "9. Thoat chuong trinh\n";
            std::cout << "Lua chon cua ban (1-9): ";

            if (!(std::cin >> choice)) {
                std::cout << "Loi: Vui long nhap so nguyen tu 1 den 9!\n";
                clearInputBuffer();
                continue;
            }

            switch (choice) {
                case 1: createParkingLot(); break;
                case 2: addVehicleToSystem(); break;
                case 3: removeParkingLot(); break;
                case 4: removeVehicleFromSystem(); break;
                case 5: displayAllLots(); break;
                case 6: displaySpecificLot(); break;
                case 7: listVehiclesByType(); break;
                case 8: summarizeAverageFees(); break;
                case 9: std::cout << "Cam on ban da su dung chuong trinh!\n"; break;
                default: std::cout << "Vui long chon lai trong khoang (1-9)!\n"; break;
            }
        } while (choice != 9);
    }
};

int main() {
    SystemInterface sys;
    sys.runMenu();
    return 0;
}
