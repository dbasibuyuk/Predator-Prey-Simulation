#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>


using namespace std;

const int gridSize = 64; // the size has to be power of 4.(the shape is square)
const int chanceOfPoison = 10; // it is percentage, so  10% for now.
const int numOfDoodlebugs = 5;
const int numOfAnt = 100;
void initialize(); // initialize the grid

class Organism
{
    private:
        int h, v; // position of the organism
        int lifeTime; 
    public:
        virtual void move() = 0;
        virtual void breed() = 0;

        int getLifeTime();
        void setLifeTime();
        void setLifeTime(int x);

        void setV(int a);
        void setH(int a);
        int getV();
        int getH();


        void setLocation(int v, int h);

};

class Ant : public Organism
{
        public:
        void move();
        void breed();
        Ant();
        Ant(int v, int h);
};


class PoisAnt : public Ant
{
    public:
        void move();
        void breed();
        PoisAnt();
        PoisAnt(int v, int h);
};  

class DoodleBug : public Organism
{
    int death; // when it is 0, it dies
    int poisoned; // when it is 1, it dies after 2 moves
    int lastMove; // when get poisoned, it counts back to 0
    public:
        void move();
        void breed();
        int starve();
        int getPoisoned(); // it deletes 
        DoodleBug();
        DoodleBug(int v, int h);
        
};


class Grid
{
    private:
        char** grid;
    public:
        Grid(); // constructor
        ~Grid(); // destructor
        Grid(const Grid& object); // copy constructor
        Grid& operator=(const Grid& rightSide); // assignment operator
        void print();

        char getGrid(int v, int h);
        void setGrid(char value, int v, int h);

};

Grid grid; // define a grid in global so that we can use in other classes.
vector <Ant> ants;
vector <PoisAnt> poisonAnts; 
vector <DoodleBug> doodlebugs;


int main()
{
    srand(time(NULL));
    initialize();
    char flag;
    do
    {

        grid.print();
        cout << endl;
        cout << "Ants : " << ants.size() << "\t";
        cout << "Poisonous Ants : " << poisonAnts.size() << "\t";
        cout << "Doodlebugs : " << doodlebugs.size() << endl << endl;

        // doodlebugs move
        for(int i = 0; i < doodlebugs.size(); i++)
        {
            doodlebugs[i].move();
            int del1 = doodlebugs[i].getPoisoned();
            if(del1 != -1)
            {
                doodlebugs.erase(doodlebugs.begin() + del1);
            }
            int del = doodlebugs[i].starve();
            if(del != -1)
            {
                doodlebugs.erase(doodlebugs.begin() + del);
            }

            if(doodlebugs[i].getLifeTime() == 8)
            {
                doodlebugs[i].breed();
            }

        }

        // ants move
        for(int i = 0; i < ants.size(); i++)
        {
            ants[i].move();
            if(ants[i].getLifeTime() == 3)
            {
                ants[i].breed();
            }
        }

        // poisonous ants move
        for(int i = 0; i < poisonAnts.size(); i++)
        {
            poisonAnts[i].move();
            if(poisonAnts[i].getLifeTime() == 4)
            {
                poisonAnts[i].breed();
            }
        }


        flag = cin.get();
    }while(flag == '\n');
    return 0;
}


Grid::Grid()
{
    grid = new char*[gridSize / 4];

    for(int i = 0; i < gridSize / 4; i++)
    {
        grid[i] = new char[gridSize / 4];
    }

    for(int i = 0; i < gridSize / 4; i++)
    {
        for(int k = 0; k < gridSize / 4; k++)
        {
            grid[i][k] = '-';
        }
    }
}

void Grid::print()
{
    for(int i = 0; i < gridSize / 4; i++)
    {
        for(int k = 0; k < gridSize / 4; k++)
        {
            cout << grid[i][k] << " ";
        }

        cout << endl;
    }
}

Grid::~Grid()
{
    for(int i = 0; i < gridSize / 4; i++)
    {
        delete [] grid[i];
    }

    delete [] grid;
}

Grid::Grid(const Grid& object)
{
    grid = new char*[gridSize / 4];

    for(int i = 0; i < gridSize / 4; i++)
    {
        grid[i] = new char[gridSize / 4];
    }

    for(int i = 0; i < gridSize / 4; i++)
    {
        for(int k = 0; k < gridSize / 4; k++)
        {
            grid[i][k] = object.grid[i][k];
        }
    }
}

Grid& Grid::operator=(const Grid& rightSide)
{
    for(int i = 0; i < gridSize / 4; i++)
    {
        for(int k = 0; k < gridSize / 4; k++)
        {
            grid[i][k] = rightSide.grid[i][k];
        }
    }

    return *this;
}

void Ant::move()
{
    int direction = rand() % 4;


    // to left
    if(direction == 0)
    {
        if(getH() != 0 && grid.getGrid(getV(), getH() - 1) == '-')
        {
            grid.setGrid('o', getV(), getH() - 1);
            grid.setGrid('-', getV(), getH());
            setLifeTime();

            setH(getH() - 1);
        }
    }
    
    // to up
    else if(direction == 1)
    {
        if(getV() != 0 && grid.getGrid(getV() - 1, getH()) == '-')
        {
            grid.setGrid('o', getV() - 1, getH());
            grid.setGrid('-', getV(), getH());
            setLifeTime();

            setV(getV() - 1);
        }
    }
    
    // to right
    else if(direction == 2)
    {  
        if(getH() != ((gridSize / 4) - 1) && grid.getGrid(getV(), getH() + 1) == '-')
        {
            grid.setGrid('o', getV(), getH() + 1);
            grid.setGrid('-', getV(), getH());
            setLifeTime();

            setH(getH() + 1);
        }
    }

    // to down
    else if(direction == 3)
    {
        if(getV() != ((gridSize / 4) - 1) && grid.getGrid(getV() + 1, getH()) == '-')
        {
            grid.setGrid('o', getV() + 1, getH());
            grid.setGrid('-', getV(), getH());
            setLifeTime();

            setV(getV() + 1);
        }
    }
}

void Ant::breed()
{
    int direction = rand() % 4;
    int chance = rand() % 100;
    bool poison;

    if(chanceOfPoison > chance)
    {
        poison = true;
    }
    else
    {
        poison = false;
    }

    // to left
    if(direction == 0)
    {
        if(getH() != 0 && grid.getGrid(getV(), getH() - 1) == '-')
        {
            setLifeTime(0);

            if(poison)
            {
                poisonAnts.push_back(PoisAnt(getV(), getH() - 1));
            }
            else
            {
                ants.push_back(Ant(getV(), getH() - 1));
            }
        }
    }
    
    // to up
    else if(direction == 1)
    {
        if(getV() != 0 && grid.getGrid(getV() - 1, getH()) == '-')
        {
            setLifeTime(0);

            if(poison)
            {
                poisonAnts.push_back(PoisAnt(getV() - 1,  getH()));
            }
            else
            {
                ants.push_back(Ant(getV() - 1,  getH()));
            }
        }
    }
    
    // to right
    else if(direction == 2)
    {  
        if(getH() != ((gridSize / 4) - 1) && grid.getGrid(getV(), getH() + 1) == '-')
        {
            setLifeTime(0);

            if(poison)
            {
                poisonAnts.push_back(PoisAnt(getV(), getH() + 1));
            }
            else
            {
                ants.push_back(Ant(getV(), getH() + 1));
            }
        }
    }

    // to down
    else if(direction == 3)
    {
        if(getV() != ((gridSize / 4) - 1) && grid.getGrid(getV() + 1, getH()) == '-')
        {
            setLifeTime(0);

            if(poison)
            {
                poisonAnts.push_back(PoisAnt(getV() + 1, getH()));
            }
            else
            {
                ants.push_back(Ant(getV() + 1, getH()));
            }
        }
    }
}



char Grid::getGrid(int v, int h)
{
    return grid[v][h];
}

void Grid::setGrid(char value, int v, int h)
{
    grid[v][h] = value;
}

Ant::Ant(int v, int h)
{
    setLifeTime(0);
    grid.setGrid('o', v, h);
    setLocation(v, h);
}

Ant::Ant()
{
    setLifeTime(0);
}






PoisAnt::PoisAnt(int v, int h)
{
    setLifeTime(0);
    grid.setGrid('c', v, h);
    setLocation(v, h);
}

PoisAnt::PoisAnt()
{
    setLifeTime(0);
}

void PoisAnt::move()
{
    int direction = rand() % 4;

    // to left
    if(direction == 0)
    {
        if(getH() != 0 && grid.getGrid(getV(), getH() - 1) == '-')
        {
            grid.setGrid('c', getV(), getH() - 1);
            grid.setGrid('-', getV(), getH());
            setLifeTime();

            setH(getH() - 1);
        }
    }
    
    // to up
    else if(direction == 1)
    {
        if(getV() != 0 && grid.getGrid(getV() - 1, getH()) == '-')
        {
            grid.setGrid('c', getV() - 1, getH());
            grid.setGrid('-', getV(), getH());
            setLifeTime();

            setV(getV() - 1);
        }
    }
    
    // to right
    else if(direction == 2)
    {  
        if(getH() != ((gridSize / 4) - 1) && grid.getGrid(getV(), getH() + 1) == '-')
        {
            grid.setGrid('c', getV(), getH() + 1);
            grid.setGrid('-', getV(), getH());
            setLifeTime();

            setH(getH() + 1);
        }
    }

    // to down
    else if(direction == 3)
    {
        if(getV() != ((gridSize / 4) - 1) && grid.getGrid(getV() + 1, getH()) == '-')
        {
            grid.setGrid('c', getV() + 1, getH());
            grid.setGrid('-', getV(), getH());
            setLifeTime();

            setV(getV() + 1);
        }
    }
}

void PoisAnt::breed()
{
    int empty;
    vector <int> emptyCell;

    // looking for empty cell

    //left
    if(getH() != 0 && grid.getGrid(getV(), getH() - 1) == '-')
    {
        emptyCell.push_back(0);
    }
    // up
    else if(getV() != 0 && grid.getGrid(getV() - 1, getH()) == '-')
    {
        emptyCell.push_back(1);
    }
    // right
    else if(getH() != ((gridSize / 4) - 1) && grid.getGrid(getV(), getH() + 1) == '-')
    {
        emptyCell.push_back(2);
    }
    // down
    else if(getV() != ((gridSize / 4) - 1) && grid.getGrid(getV() + 1, getH()) == '-')
    {
        emptyCell.push_back(3);
    }


    // there is no empty cell, so it has to fight
    if(emptyCell.size() == 0)
    {
        // breed to left if it is possible
        if(getH() != 0)
        {
            setLifeTime(0);

            // clean the dead organism
            int cleaned = 0;
            for(int i = 0; i < ants.size(); i++)
            {
                if(ants[i].getV() == getV() && ants[i].getH() == getH() - 1)
                {
                    ants.erase(ants.begin() + i);
                    cleaned = 1;
                }
            }
            if(cleaned == 0)
            {
                for(int i = 0; i < poisonAnts.size(); i++)
                {
                    if(poisonAnts[i].getV() == getV() && poisonAnts[i].getH() == getH() - 1)
                    {
                        poisonAnts.erase(poisonAnts.begin() + i);
                        cleaned = 1;
                    }
                }
            }
            if(cleaned == 0)
            {   
                for(int i = 0; i < doodlebugs.size(); i++)
                {
                    if(doodlebugs[i].getV() == getV() && doodlebugs[i].getH() == getH() - 1)
                    {
                        doodlebugs.erase(doodlebugs.begin() + i);
                        cleaned = 1;
                    }
                } 
            }  

            poisonAnts.push_back(PoisAnt(getV(), getH() - 1));

            
        }
        // breed to up
        else if(getV() != 0)
        {
            setLifeTime(0);
            int cleaned = 0;
            // clean the dead organism
            for(int i = 0; i < ants.size(); i++)
            {
                if(ants[i].getV() == getV() - 1 && ants[i].getH() == getH())
                {
                    ants.erase(ants.begin() + i);
                    cleaned = 1;
                }
            }
            
            if(cleaned == 0)
            {
                for(int i = 0; i < poisonAnts.size(); i++)
                {
                    if(poisonAnts[i].getV() == getV() -1 && poisonAnts[i].getH() == getH())
                    {
                        poisonAnts.erase(poisonAnts.begin() + i);
                        cleaned = 1;
                    }
                }
            }

            if(cleaned == 0)
            {
                for(int i = 0; i < doodlebugs.size(); i++)
                {
                    if(doodlebugs[i].getV() == getV() -1 && doodlebugs[i].getH() == getH())
                    {
                        doodlebugs.erase(doodlebugs.begin() + i);
                        cleaned = 1;
                    }
                }
            }
            




            poisonAnts.push_back(PoisAnt( getV() - 1, getH()));
        }
        // breed to right
        else if(getH() != ((gridSize / 4) - 1))
        {
            int cleaned = 0;
            setLifeTime(0);

            // clean the dead organism
            for(int i = 0; i < ants.size(); i++)
            {
                if(ants[i].getV() == getV() && ants[i].getH() == getH() + 1)
                {
                    ants.erase(ants.begin() + i);
                    cleaned = 1;
                }
            }
            if(cleaned == 0)
            {
                for(int i = 0; i <poisonAnts.size(); i++)
                {
                    if(poisonAnts[i].getV() == getV() && poisonAnts[i].getH() == getH() + 1)
                    {
                        poisonAnts.erase(poisonAnts.begin() + i);
                        cleaned = 1;
                    }
                }
            }

            if(cleaned == 0)
            {
                for(int i = 0; i < doodlebugs.size(); i++)
                {
                    if(doodlebugs[i].getV() == getV() && doodlebugs[i].getH() == getH() + 1)
                    {
                        doodlebugs.erase(doodlebugs.begin() + i);
                    }
                }
            }

            poisonAnts.push_back(PoisAnt(getV(), getH() + 1));
        }
        // breed to down
        else if(getV() != ((gridSize / 4) - 1))
        {
            setLifeTime(0);
            int cleaned = 0;

            // clean the dead organism
            for(int i = 0; i < ants.size(); i++)
            {
                if(ants[i].getV() == getV() + 1 && ants[i].getH() == getH())
                {
                    ants.erase(ants.begin() + i);
                    cleaned = 1;
                }
            }
            if(cleaned == 0)
            {
                for(int i = 0; i < poisonAnts.size(); i++)
                {
                    if(poisonAnts[i].getV() == getV() + 1 && poisonAnts[i].getH() == getH())
                    {
                        poisonAnts.erase(poisonAnts.begin() + i);
                        cleaned = 1;
                    }
                }
            }
            if(cleaned == 0)
            {
                for(int i = 0; i < doodlebugs.size(); i++)
                {
                    if(doodlebugs[i].getV() == getV() + 1 && doodlebugs[i].getH() == getH())
                    {
                        doodlebugs.erase(doodlebugs.begin() + i);
                        cleaned = 1;
                    }
                }
            }
            

            poisonAnts.push_back(PoisAnt(getV() + 1, getH()));
        }
    }
    else  // there is empty cell
    {
        int direction = rand() % emptyCell.size();

        // left
        if(emptyCell[direction] == 0)
        {
            setLifeTime(0);
            poisonAnts.push_back(PoisAnt(getV(), getH() - 1));
        }
        // up
        else if(emptyCell[direction] == 1)
        {
            setLifeTime(0);
            poisonAnts.push_back(PoisAnt(getV() - 1, getH()));
        }
        // right
        else if(emptyCell[direction] == 2)
        {
            setLifeTime(0);
            poisonAnts.push_back(PoisAnt(getV(), getH() + 1));
        }
        // down
        else if(emptyCell[direction] == 3)
        {
            setLifeTime(0);
            poisonAnts.push_back(PoisAnt(getV() + 1, getH()));
        }
    }
}













int Organism::getLifeTime()
{
    return lifeTime;
}

void Organism::setLifeTime()
{
    lifeTime += 1;
}


void Organism::setLocation(int v, int h)
{
    this -> v = v;
    this -> h = h;
}

void Organism::setLifeTime(int x)
{
    lifeTime = x;
}

void Organism::setV(int a)
{
    v = a;
}
void Organism::setH(int a)
{
    h = a;
}
int Organism::getV()
{
    return v;
}
int Organism::getH()
{
    return h;
}













DoodleBug::DoodleBug()
{
    death = 3;
    lastMove = 2;
    setLifeTime(0);
}
DoodleBug::DoodleBug(int v, int h)
{
    death = 3;
    lastMove = 2;
    setLifeTime(0);
    grid.setGrid('X', v, h);
    setLocation(v, h);
}

void DoodleBug::move()
{
    int flag = 0;
    // looking for food 

    // to left
    if(getH() != 0)
    {
        // if poisonous or normal ant is found
        if(grid.getGrid(getV(), getH() - 1) == 'c' || grid.getGrid(getV(), getH() - 1) == 'o')
        {
            for(int i = 0; i < ants.size(); i++)
            {
                if(ants[i].getV() == getV() && ants[i].getH() == getH() - 1)
                {
                    ants.erase(ants.begin() + i);
                    death = 3;
                }
            }

            for(int i = 0; i < poisonAnts.size(); i++)
            {
                if(poisonAnts[i].getV() == getV() && poisonAnts[i].getH() == getH() - 1)
                {
                    poisonAnts.erase(poisonAnts.begin() + i);
                    poisoned = 1;
                }
            }

            grid.setGrid('X', getV(), getH() - 1);
            grid.setGrid('-', getV(), getH());
            setLifeTime();

            setH(getH() - 1);

            flag = 1;
        }
    }
    // to up
    if(flag == 0)
    {
        if(getV() != 0)
        {
            if(grid.getGrid(getV() - 1, getH()) == 'c' || grid.getGrid(getV() - 1, getH()) == 'o')
            {
                for(int i = 0; i < ants.size(); i++)
                {
                    if(ants[i].getV() == getV() - 1 && ants[i].getH() == getH())
                    {
                        ants.erase(ants.begin() + i);
                        death = 3;
                    }
                }
                

                for(int i = 0; i < poisonAnts.size(); i++)
                {
                    if(poisonAnts[i].getV() == getV() -1 && poisonAnts[i].getH() == getH())
                    {
                        poisonAnts.erase(poisonAnts.begin() + i);
                        poisoned = 1;
                    }
                }

                grid.setGrid('X', getV() - 1, getH());
                grid.setGrid('-', getV(), getH());
                setLifeTime();

                setV(getV() - 1);


                flag = 1;
            }



        }
    }
    
    if(flag == 0)
    {
        // to right 
        if(getH() != ((gridSize / 4) - 1))
        {
            if(grid.getGrid(getV(), getH() + 1) == 'c' || grid.getGrid(getV(), getH() + 1) == 'o')
            {
                for(int i = 0; i < ants.size(); i++)
                {
                    if(ants[i].getV() == getV() && ants[i].getH() == getH() + 1)
                    {
                        death = 3;
                        ants.erase(ants.begin() + i);
                    }
                }

                for(int i = 0; i <poisonAnts.size(); i++)
                {
                    if(poisonAnts[i].getV() == getV() && poisonAnts[i].getH() == getH() + 1)
                    {
                        poisonAnts.erase(poisonAnts.begin() + i);
                        poisoned = 1;
                    }
                }

                grid.setGrid('X', getV(), getH() + 1);
                grid.setGrid('-', getV(), getH());
                setLifeTime();

                setH(getH() + 1);




                flag = 1;
            }

        }
    }
    
    if(flag == 0)
    {
        // to down
        if(getV() != ((gridSize / 4) - 1))
        {
            if(grid.getGrid(getV() + 1, getH()) == 'c' || grid.getGrid(getV() + 1, getH()) == 'o')
            {
                for(int i = 0; i < ants.size(); i++)
                {
                    if(ants[i].getV() == getV() + 1 && ants[i].getH() == getH())
                    {
                        death = 3;
                        ants.erase(ants.begin() + i);
                    }
                }

                for(int i = 0; i < poisonAnts.size(); i++)
                {
                    if(poisonAnts[i].getV() == getV() + 1 && poisonAnts[i].getH() == getH())
                    {
                        poisonAnts.erase(poisonAnts.begin() + i);
                        poisoned = 1;
                    }
                }

                grid.setGrid('X', getV() + 1, getH());
                grid.setGrid('-', getV(), getH());
                setLifeTime();

                setV(getV() + 1);



                flag = 1;
            }
        }
    }

    // if there is nothing to eat
    if(flag == 0)
    {
        int direction = rand() % 4;

        // to left
        if(direction == 0)
        {
            if(getH() != 0 && grid.getGrid(getV(), getH() - 1) == '-')
            {
                grid.setGrid('X', getV(), getH() - 1);
                grid.setGrid('-', getV(), getH());
                setLifeTime();
                death--;
                setH(getH() - 1);
            }
        }
        
        // to up
        else if(direction == 1)
        {
            if(getV() != 0 && grid.getGrid(getV() - 1, getH()) == '-')
            {
                grid.setGrid('X', getV() - 1, getH());
                grid.setGrid('-', getV(), getH());
                setLifeTime();
                death--;
                setV(getV() - 1);
            }
        }
        
        // to right
        else if(direction == 2)
        {  
            if(getH() != ((gridSize / 4) - 1) && grid.getGrid(getV(), getH() + 1) == '-')
            {
                grid.setGrid('X', getV(), getH() + 1);
                grid.setGrid('-', getV(), getH());
                setLifeTime();
                death--;
                setH(getH() + 1);
            }
        }

        // to down
        else if(direction == 3)
        {
            if(getV() != ((gridSize / 4) - 1) && grid.getGrid(getV() + 1, getH()) == '-')
            {
                grid.setGrid('X', getV() + 1, getH());
                grid.setGrid('-', getV(), getH());
                setLifeTime();
                death--;
                setV(getV() + 1);
            }
        }

    }
}
 
void DoodleBug::breed()
{
    int direction = rand() % 4;

    // to left
    if(direction == 0)
    {
        if(getH() != 0 && grid.getGrid(getV(), getH() - 1) == '-')
        {
            setLifeTime(0);

            doodlebugs.push_back(DoodleBug(getV(), getH() - 1));
        }
    }
    
    // to up
    else if(direction == 1)
    {
        if(getV() != 0 && grid.getGrid(getV() - 1, getH()) == '-')
        {
            setLifeTime(0);

            doodlebugs.push_back(DoodleBug(getV() - 1, getH()));
        }
    }
    
    // to right
    else if(direction == 2)
    {  
        if(getH() != ((gridSize / 4) - 1) && grid.getGrid(getV(), getH() + 1) == '-')
        {
            setLifeTime(0);

            doodlebugs.push_back(DoodleBug(getV(), getH() + 1));
        }
    }

    // to down
    else if(direction == 3)
    {
        if(getV() != ((gridSize / 4) - 1) && grid.getGrid(getV() + 1, getH()) == '-')
        {
            setLifeTime(0);

            doodlebugs.push_back(DoodleBug(getV() + 1, getH()));
        }
    }
}

int DoodleBug::starve()
{
    if(death == 0)
    {
        for(int i = 0; i < doodlebugs.size(); i++)
        {
            if(doodlebugs[i].getV() == getV() && doodlebugs[i].getH() == getH())
            {
                grid.setGrid('-', doodlebugs[i].getV(), doodlebugs[i].getH());
                return i;
            }
        }
    }
    else
    {
        return -1;
    }

}

int DoodleBug::getPoisoned()
{
    // if it is poisoned
    if(poisoned == 1)
    {
        lastMove--;
        if(lastMove == 0)
        {
            for(int i = 0; i < doodlebugs.size(); i++)
            {
                if(getV() == doodlebugs[i].getV() && getH() == doodlebugs[i].getH())
                {
                    grid.setGrid('-', doodlebugs[i].getV(), doodlebugs[i].getH());
                    return i;
                }
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

void initialize()
{
    for(int i = 0; i < numOfAnt; i++)
    {
        int flag = 0;
        do
        {
            int locationV = rand() % (gridSize / 4);
            int locationH = rand() % (gridSize / 4);

            if(grid.getGrid(locationV, locationH) == '-')
            {
                ants.push_back(Ant(locationV, locationH));
                flag = 1;
            }

        } while(flag == 0);
    }

    for(int i = 0; i < numOfDoodlebugs; i++)
    {
        int flag = 0;
        do
        {
            int locationV = rand() % (gridSize / 4);
            int locationH = rand() % (gridSize / 4);

            if(grid.getGrid(locationV, locationH) == '-')
            {
                doodlebugs.push_back(DoodleBug(locationV, locationH));
                flag = 1;
            }

        } while(flag == 0);
    }
}