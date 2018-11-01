#include <iostream>
#include <ctime>
#include <cmath>

#define POP_SIZE 10


/*=*=*=*=*=*=*=*=*=*=* Classes *=*=*=*=*=*=*=*=*=*=*/

class Indiv{
    private:
        double fit;
    public: 
        double x;
        double y;
        Indiv() : fit(-2048) {};
        Indiv(double a, double b):
            x(a),
            y(b),
            fit(-2048)
    { };
        double get_fitness(){
            if (fit == -2048){
                // -20 * exp(-0.2 * (sqrt(0.5*(x * x + y * y)))) - exp(0.5 * (cos(2 * pi * x) + cos(2 * pi * y))) + e + 20
                fit = (-20 * exp(-0.2 * (sqrt(0.5 * x * x + y * y)))) - (exp(0.5 * (cos(2 * M_PI * x) + cos(2 * M_PI * y)))) + M_E + 20;
            }
            return fit;
        }
};

/*=*=*=*=*=*=*=*=*=*=* Function Declarations *=*=*=*=*=*=*=*=*=*=*/

/* Creates the population and assigns random values to its members*/ 
Indiv** init_pop();
 
/* Sorts the population by fitness */
void bubble_fit_sort(Indiv**);

/* Returns a new population whose individuals are the 'children' of the best of the previous population with another member of the previous population */
Indiv** elitismo(Indiv**);

/*=*=*=*=*=*=*=*=*=*=* Global Variables *=*=*=*=*=*=*=*=*=*=*/

/* Controls how much the parameters of the 'children' mutate with each generation */
int mut = 2;

/*=*=*=*=*=*=*=*=*=*=* Main *=*=*=*=*=*=*=*=*=*=*/

int main(){

    double first_best;

    double prev_best;
    int gen_counter;
    int repeat_best_count;

    Indiv** pop = init_pop();
    bubble_fit_sort(pop);

    first_best = pop[0] -> get_fitness();
    
    gen_counter = 0;
    std::cout << "gen " << gen_counter << "; best fitness: " << std::fixed << pop[0] -> get_fitness() << std::endl;
    gen_counter++;

    prev_best = pop[0] -> get_fitness();
    repeat_best_count = 0;

    while (pop[0] -> get_fitness() > 0.00005){
        pop = elitismo(pop);
        bubble_fit_sort(pop);
        std::cout << "gen " << gen_counter << ";\tbest: " << std::fixed <<  pop[0] -> get_fitness() << "(" << std::fixed <<  pop[0] -> x << "," << std::fixed << pop[0] -> y << ");\tcurrent mut: " << mut << std::endl;

        gen_counter++;
        
        /* If the best fitness stays the same for 20 generations, this adds 2 to the mut variable or makes it zero if it is already 98 */
        if (pop[0] -> get_fitness() == prev_best)
            repeat_best_count++;
        else {
            prev_best = pop[0] -> get_fitness();
            repeat_best_count = 0;
        }
        if (repeat_best_count == 20){
            mut = (mut + 2) % 100;
            repeat_best_count = 0;
        }

    }

    std::cout << "Went from " << std::fixed << first_best << " to " << std::fixed << pop[0] -> get_fitness() << " in " << gen_counter << " generations.\n";

};

/*=*=*=*=*=*=*=*=*=*=* Function Definitions *=*=*=*=*=*=*=*=*=*=*/

Indiv** init_pop(){

    Indiv** pop; 
    pop = new Indiv* [POP_SIZE];

    srand(time(NULL));


    for (int i = 0; i < POP_SIZE; i++){
        pop[i] = new Indiv;
        pop[i] -> x = (rand() % 20) - 10;
        pop[i] -> y = (rand() % 20) - 10;
    }
    return pop;
}

void swap(Indiv** xp, Indiv** yp) { 
    Indiv* temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 

void bubble_fit_sort(Indiv** pop){

    bool swapped; 
    for (int i = 0; i < POP_SIZE - 1; i++) 
    { 
        swapped = false; 
        for (int j = 0; j < POP_SIZE - i - 1; j++){ 
            if ((pop[j] -> get_fitness()) > (pop[j+1] -> get_fitness())){ 
                swap(&(pop[j]), &(pop[j+1]));
                swapped = true; 
            } 
        } 

        if (swapped == false) 
            break; 
    } 

}

void torneio_de_dois(Indiv* pop){
    /*     Indiv best = pop[0]; */
    /*     Indiv* new_pop = malloc(sizeof(Indiv) * POP_SIZE); */
    /*     for (int i = 1; i < POP_SIZE; i++){ */
    /*         if (fitness(pop[i]) < fitness(best)) */
    /*             best = pop[i]; */
    /*     } */ 
    /*     new_pop[0] = best; */

    /*     for (int i = 1; i < POP_SIZE; i++){ */
    /*         Indiv mom, dad; */
    /*         int comp_a, comp_b; */
    /*         comp_a = rand() % POP_SIZE; */
    /*         comp_b = rand() % POP_SIZE; */
    /*         if (fitness(pop[comp_a]) < fitness(pop[comp_b])) */
    /*             mom = comp_a; */
    /*         else mom = comp_b; */

    /*         comp_a = rand() % POP_SIZE; */
    /*         comp_b = rand() % POP_SIZE; */
    /*         if (fitness(pop[comp_a]) < fitness(pop[comp_b])) */
    /*             dad = comp_a; */
    /*         else dad = comp_b; */

    /*         new_pop[i] = (mom + dad) / 2; */
    /*     } */ 

    /*     return new_pop; */
}

Indiv** elitismo(Indiv** pop){
    Indiv* best = pop[0];
    Indiv** new_pop = new Indiv* [POP_SIZE];

    bubble_fit_sort(pop);

    new_pop[0] = pop[0];

    for (int i = 1; i < POP_SIZE; i++){
        new_pop[i] = new Indiv;
        new_pop[i] -> x = (pop[i] -> x + pop[0] -> x) / 2 + (mut * ((rand() % 200) - 100)/100.0)/100.0;
        new_pop[i] -> y = (pop[i] -> y + pop[0] -> y) / 2 + (mut * ((rand() % 200) - 100)/100.0)/100.0;
        delete pop[i];
    }
    delete pop;

    return new_pop;
}





