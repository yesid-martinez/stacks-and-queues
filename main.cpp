#include <iostream>
#include <string>
#include <cctype> // Para usar isdigit
#include <sstream> // Para la conversión de tipos

using namespace std;

struct Product {
    int orderId;           // ID de orden
    string productName;     // Nombre del producto
    int quantAvail;         // Cantidad disponible
    int unitPrice;          // Precio por unidad
    int ivaPercentage;      // IVA en porcentaje
};

struct Node {
    Product product;
    Node* next;
};

// Punteros para la pila y la cola
Node* watchStack = nullptr;   // Pila de relojes
Node* pizzaQueueFront = nullptr; // Cola de pizzas
Node* pizzaQueueRear = nullptr;

int cash = 100000000;
int watchPrice = 60000;
int pizzaPrice = 35000;
int currentWatchId = 1;  
int currentPizzaId = 1;
int totalOrders = 0;

int option = 0;

void printOrderCount();
void searchOrder();

bool isNum(const string& str){
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

// Agregar un reloj a la pila
void pushWatch(int quantity) {
    for (int i = 0; i < quantity; i++) {
        Node* newNode = new Node();
        newNode->product = {currentWatchId++, "Reloj", 1, watchPrice, 19};
        newNode->next = watchStack;
        watchStack = newNode;
    }
    cout << quantity << " Reloj(es) anadidos al almacenamiento." << endl;
}

// Agregar una pizza a la cola
void enqueuePizza(int quantity) {
    for (int i = 0; i < quantity; i++) {
        Node* newNode = new Node();
        newNode->product = {currentPizzaId++, "Pizza", 1, pizzaPrice, 12};
        newNode->next = nullptr;

        if (pizzaQueueRear == nullptr) {
            pizzaQueueFront = pizzaQueueRear = newNode;
        } else {
            pizzaQueueRear->next = newNode;
            pizzaQueueRear = newNode;
        }
    }
    cout << quantity << " Pizza(s) anadidas al almacenamiento." << endl;
}

void buyProduct() {
    string choice;
    int quantity, totalCost;

    while (true) {
        cout << "Seleccione el producto para comprar:\n1. Reloj\n2. Pizza\n";
        cin >> choice;

        if (isNum(choice)) {  // Validar que la entrada sea numérica
            int option;
            stringstream ss(choice);
            ss >> option;

            if (option == 1 || option == 2) {  // Validar que la opción sea 1 o 2
                cout << "Ingrese la cantidad: ";
                cin >> choice;

                if (isNum(choice)) {  // Validar que la cantidad sea numérica
                    stringstream ssQuantity(choice);
                    ssQuantity >> quantity;

                    if (quantity > 0) {  // Validar que sea un número positivo
                        if (option == 1) { // Comprar relojes
                            totalCost = quantity * watchPrice * 1.19;
                            if (cash >= totalCost) {
                                cash -= totalCost;
                                pushWatch(quantity);
                                cout << "Compra exitosa. Total gastado: $" << totalCost << endl;
                            } else {
                                cout << "Fondos insuficientes para completar la compra." << endl;
                            }
                        } else if (option == 2) { // Comprar pizzas
                            totalCost = quantity * pizzaPrice * 1.12;
                            if (cash >= totalCost) {
                                cash -= totalCost;
                                enqueuePizza(quantity);
                                cout << "Compra exitosa. Total gastado: $" << totalCost << endl;
                            } else {
                                cout << "Fondos insuficientes para completar la compra." << endl;
                            }
                        }
                        break;  // Salir del bucle una vez se complete la compra
                    } else {
                        cout << "La cantidad debe ser un numero entero positivo.\n";
                    }
                } else {
                    cout << "Entrada invalida. Por favor, ingrese un numero entero positivo.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
            } else {
                cout << "Opcion invalida. Por favor, seleccione 1 para Reloj o 2 para Pizza.\n";
            }
        } else {
            cout << "Entrada invalida. Por favor, ingrese un numero.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

void printStorage() {
	
	// Verifica si ambos inventarios están vacíos
    if (watchStack == nullptr && pizzaQueueFront == nullptr) {
        cout << "No hay productos disponibles en el almacenamiento." << endl;
        return; // Salir de la función si no hay productos
    }
	
    cout << "Inventario de Relojes (Pila):" << endl;
    Node* temp = watchStack;
    while (temp != nullptr) {
        cout << "ID: " << temp->product.orderId << ", Nombre: " << temp->product.productName
             << ", Precio: $" << temp->product.unitPrice << ", IVA: " << temp->product.ivaPercentage << "%" << endl;
        temp = temp->next;
    }

    cout << "\nInventario de Pizzas (Cola):" << endl;
    temp = pizzaQueueFront;
    while (temp != nullptr) {
        cout << "ID: " << temp->product.orderId << ", Nombre: " << temp->product.productName
             << ", Precio: $" << temp->product.unitPrice << ", IVA: " << temp->product.ivaPercentage << "%" << endl;
        temp = temp->next;
    }
}

int countWatches() {
    int count = 0;
    Node* current = watchStack;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

int countPizzas() {
    int count = 0;
    Node* current = pizzaQueueFront;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}


void sellProduct() {
    string choice;

    // Verificar si no hay nada en el inventario antes de mostrar opciones
    if (watchStack == nullptr && pizzaQueueFront == nullptr) {
        cout << "No hay productos en el inventario para vender." << endl;
        return;
    }

    while (true) {
        cout << "Seleccione el producto para vender:\n1. Reloj\n2. Pizza\n";
        cin >> choice;

        if (isNum(choice)) {  // Validar que la entrada sea numérica
            int option;
            stringstream ss(choice);
            ss >> option;

            if (option == 1) {  // Opción 1: Vender reloj
                if (watchStack == nullptr) {
                    cout << "No hay relojes en el inventario para vender." << endl;
                    return;
                }

                int availableWatches = countWatches();  // Contar relojes disponibles
                int quantity;

                cout << "Cantidad de relojes a vender (disponibles: " << availableWatches << "): ";
                cin >> quantity;

                if (cin.fail() || quantity <= 0) {  // Validar que la cantidad sea un entero positivo
                    cout << "Entrada invalida. Por favor, ingrese un numero entero positivo.\n";
                    cin.clear();            // Limpiar el error en `cin`
                    cin.ignore(10000, '\n'); // Ignorar el resto de la línea
                    continue;
                }

                // Verificar si hay suficientes relojes en el inventario
                if (quantity > availableWatches) {
                    cout << "Solo hay " << availableWatches << " relojes disponibles. Vendiendo esa cantidad..." << endl;
                    quantity = availableWatches;  // Vender solo lo que hay
                }

                // Procesar la venta de relojes
                for (int i = 0; i < quantity; ++i) {
                    int sellPrice = watchStack->product.unitPrice * 1.21;  // 21% de IVA para relojes
                    cash += sellPrice;
                    Node* temp = watchStack;
                    watchStack = watchStack->next;
                    totalOrders++;  // Aumentar el contador de pedidos
                    delete temp;
                	cout <<"Reloj vendido por $" << sellPrice << ". Ganancia anadida a efectivo." << endl;
                }
                break;

            } else if (option == 2) {  // Opción 2: Vender pizza
                if (pizzaQueueFront == nullptr) {
                    cout << "No hay pizzas en el inventario para vender." << endl;
                    return;
                }

                int availablePizzas = countPizzas();  // Contar pizzas disponibles
                int quantity;

                cout << "Cantidad de pizzas a vender (disponibles: " << availablePizzas << "): ";
                cin >> quantity;

                if (cin.fail() || quantity <= 0) {  // Validar que la cantidad sea un entero positivo
                    cout << "Entrada invalida. Por favor, ingrese un numero entero positivo.\n";
                    cin.clear();            // Limpiar el error en `cin`
                    cin.ignore(10000, '\n'); // Ignorar el resto de la línea
                    continue;
                }

                // Verificar si hay suficientes pizzas en el inventario
                if (quantity > availablePizzas) {
                    cout << "Solo hay " << availablePizzas << " pizzas disponibles. Vendiendo esa cantidad..." << endl;
                    quantity = availablePizzas;  // Vender solo lo que hay
                }

                // Procesar la venta de pizzas
                for (int i = 0; i < quantity; ++i) {
                    int sellPrice = pizzaQueueFront->product.unitPrice * 1.15;  // 15% de IVA para pizzas
                    cash += sellPrice;
                    Node* temp = pizzaQueueFront;
                    pizzaQueueFront = pizzaQueueFront->next;
                    if (pizzaQueueFront == nullptr) pizzaQueueRear = nullptr; // Ajuste si la cola queda vacía
                    totalOrders++;  // Aumentar el contador de pedidos
                    cout << "Pizza vendida por $" << sellPrice << ". Ganancia anadida a efectivo." << endl;
                    delete temp;
                }
                break;

            } else {
                cout << "Opcion invalida. Por favor, seleccione 1 para Reloj o 2 para Pizza.\n";
            }
        } else {
            cout << "Entrada invalida. Por favor, ingrese un numero.\n";
            cin.clear();             // Limpiar el error en `cin`
            cin.ignore(10000, '\n'); // Ignorar el resto de la línea
        }
    }
}

void printOrderCount() {
    cout << "Cantidad total de pedidos en el almacenamiento: " << totalOrders << endl;
}

void searchOrder() {
    string choiceStr, orderIdStr;

    // Validate product choice input
    int choice;
    while (true) {
        cout << "Seleccione el producto para buscar:\n1. Reloj\n2. Pizza\n";
        cin >> choiceStr;

        if (isNum(choiceStr)) {  // Check if input is a number
            stringstream ss(choiceStr);
            ss >> choice;

            if (choice == 1 || choice == 2) {
                break;  // Valid choice
            } else {
                cout << "Opcion invalida. Por favor, seleccione 1 o 2." << endl;
            }
        } else {
            cout << "Entrada invalida. Por favor, ingrese un numero entero." << endl;
            cin.clear();            // Clear the error flag
            cin.ignore(10000, '\n'); // Ignore the rest of the line
        }
    }

    // Validate order ID input
    int orderId;
    while (true) {
        cout << "Ingrese el ID del pedido para buscar: ";
        cin >> orderIdStr;

        if (isNum(orderIdStr)) {  // Check if input is a number
            stringstream ss(orderIdStr);
            ss >> orderId;
            break;  // Valid ID
        } else {
            cout << "Entrada invalida. Por favor, ingrese un numero entero." << endl;
            cin.clear();            // Clear the error flag
            cin.ignore(10000, '\n'); // Ignore the rest of the line
        }
    }

    int count = 0;
    Node* temp;
    if (choice == 1) { // Search in the watch stack
        temp = watchStack;
        while (temp != nullptr) {
            if (temp->product.orderId == orderId) {
                cout << "Pedido de Reloj encontrado con " << count << " pedido(s) delante." << endl;
                return;
            }
            count++;
            temp = temp->next;
        }
    } else if (choice == 2) { // Search in the pizza queue
        temp = pizzaQueueFront;
        while (temp != nullptr) {
            if (temp->product.orderId == orderId) {
                cout << "Pedido de Pizza encontrado con " << count << " pedido(s) delante." << endl;
                return;
            }
            count++;
            temp = temp->next;
        }
    } else {
        cout << "Opcion invalida." << endl;
        return;
    }
    cout << "Pedido con ID " << orderId << " no encontrado." << endl;
}


// Función para limpiar el inventario y los pedidos
void clearInventory() {
    // Limpiar pila de relojes
    while (watchStack != nullptr) {
        Node* temp = watchStack;
        watchStack = watchStack->next;
        delete temp;
    }

    // Limpiar cola de pizzas
    while (pizzaQueueFront != nullptr) {
        Node* temp = pizzaQueueFront;
        pizzaQueueFront = pizzaQueueFront->next;
        delete temp;
    }
    pizzaQueueRear = nullptr; // Reiniciar la cola de pizzas
    totalOrders = 0; // Reiniciar contador de pedidos
    cout << "Inventario y pedidos eliminados con exito." << endl;
}

// Función para mostrar el menú principal
void showMenu() {
    string choice;
    
    do {
    	
        cout << "\n======== Menu ========\n";
        cout << "1. Comprar producto\n";
        cout << "2. Imprimir almacenamiento\n";
        cout << "3. Vender producto\n";
        cout << "4. Imprimir cantidad total de pedidos\n";
        cout << "5. Buscar pedido\n";
        cout << "6. Eliminar inventario y pedidos\n";
        cout << "7. Salir\n";
        cout << endl;
        cout << "Seleccione una opcion: ";
        cin >> choice;
    	cout << endl;
        
        
		if (isNum(choice)) {
            // Convertir el string a un entero
            stringstream ss(choice);
            ss >> option;
            
	            // Validar que esté dentro del rango
	            if (option >= 1 && option <= 7) {
	                switch (option) {
	                    case 1: buyProduct(); break;
	                    case 2: printStorage(); break;
	                    case 3: sellProduct(); break;
	                    case 4: printOrderCount(); break;
	                    case 5: searchOrder(); break;
	                    case 6: clearInventory(); break;
	                    case 7: cout << "Saliendo..." << endl; break;
	                }
	            } else {
		                cout << "Opcion no permitida. Ingrese un numero entre 1 y 7" << endl;
		            }
	        } else {
	            cout << "Opcion no permitida. Ingrese un numero entre 1 y 7" << endl;
        }
        
    } while (option != 7);
}

int main() {
    showMenu();
    return 0;
}
