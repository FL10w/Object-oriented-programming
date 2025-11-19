#include "Geometry.h"
#include <iostream>
#include <limits> 

using namespace std;
template <ScalarType T>
std::unique_ptr<Figure<T>> create_figure_from_input() {
    int choice;
    cout << "Enter figure type (1: Trapezoid, 2: Rhombus, 3: Pentagon, 0: Stop): ";
    
    if (!(cin >> choice) || choice == 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return nullptr;
    }

    T x1, y1, x2, y2, x3, y3, x4, y4, x5 = T{}, y5 = T{};

    switch (choice) {
        case 1: 
            cout << "Enter 4 pairs of coordinates (x1 y1 x2 y2 x3 y3 x4 y4): ";
            if (!(cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4)) return nullptr;
            return make_unique<Trapezoid<T>>(x1, y1, x2, y2, x3, y3, x4, y4);
        case 2: 
            cout << "Enter 4 pairs of coordinates (x1 y1 x2 y2 x3 y3 x4 y4): ";
            if (!(cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4)) return nullptr;
            return make_unique<Rhombus<T>>(x1, y1, x2, y2, x3, y3, x4, y4);
        case 3: 
            cout << "Enter 5 pairs of coordinates (x1 y1 x2 y2 x3 y3 x4 y4 x5 y5): ";
            if (!(cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4 >> x5 >> y5)) return nullptr;
            return make_unique<Pentagon<T>>(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5);
        default:
            cout << "Invalid figure choice." << endl;
            return nullptr;
    }
}

int main() {
    using CoordType = double; 
    Array<std::shared_ptr<Figure<CoordType>>> figure_array; 
    cout << "\n--- Interactive Figure Input ---" << endl;
    while (true) {
        cout << "\nEnter figure: ";
        if (auto fig_uptr = create_figure_from_input<CoordType>()) {
            figure_array.push_back(std::shared_ptr<Figure<CoordType>>(fig_uptr.release()));
            cout << "Figure added successfully. Total figures: " << figure_array.get_size() << endl;
        } else {
            break;
        }
    }
    
    cout << "\n--- Adding Default Figures ---" << endl;
    figure_array.push_back(std::make_shared<Rhombus<CoordType>>(1.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, -1.0));
    figure_array.push_back(std::make_shared<Trapezoid<CoordType>>(0.0, 0.0, 5.0, 0.0, 4.0, 3.0, 1.0, 3.0));
    figure_array.push_back(std::make_shared<Pentagon<CoordType>>(
        0.0, 1.0, 0.95, 0.31, 0.59, -0.81, -0.59, -0.81, -0.95, 0.31
    ));

    double total_area = 0.0;
    cout << "\n--- Processing all figures in array (" << figure_array.get_size() << " figures) ---" << endl;
    for (size_t i = 0; i < figure_array.get_size(); ++i) {
        auto& fig_ptr = figure_array[i];
        cout << "\nFigure " << i << " (" << typeid(*fig_ptr).name() << "):" << endl;
        
        Point<CoordType> center = fig_ptr->center();
        cout << "  Center: (" << center.x << ", " << center.y << ")" << endl;
        
        fig_ptr->print_coords();
        
        double area_val = static_cast<double>(*fig_ptr);
        cout << "  Area: " << area_val << endl;
        
        total_area += area_val;
        
        auto clone_ptr = fig_ptr->clone();
        if (clone_ptr) {
            cout << "  Clone area: " << static_cast<double>(*clone_ptr) << " (verification)" << endl;
        }
    }

    cout << "\nTotal area of all figures: " << total_area << endl;

    if (figure_array.get_size() > 0) {
        size_t index_to_remove = 0; 
        cout << "\nRemoving figure at index " << index_to_remove << endl;
        try {
            figure_array.remove_at(index_to_remove);
            cout << "Removal successful. New array size: " << figure_array.get_size() << endl;
        } catch (const std::out_of_range& e) {
            cout << "Error removing element: " << e.what() << endl;
        }
    }

    cout << "\n--- Testing Array template with different types ---" << endl;
    Array<std::shared_ptr<Figure<int>>> int_figures;
    Array<std::shared_ptr<Rhombus<double>>> rhombus_array;
    
    cout << "Array<Figure<int>> size: " << int_figures.get_size() << endl;
    cout << "Array<Rhombus<double>> size: " << rhombus_array.get_size() << endl;
    
    return 0;
}