#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

static const double epsilons[11] = { 0.0, 1.0, 0.5, 0.25, 0.75 };

using namespace std;

static float RandFloat() {

    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

}

class Bandit
{

    public:

        /// <summary>
        /// Количество рук
        /// </summary>
        int armsCount;

        /// <summary>
        /// Конструктор
        /// </summary>
        /// <param name="m">истинное значение среднего выигрыша на каждую ручку</param>
        Bandit(vector<double> m) {

            this->m = m;

            this->armsCount = this->m.size();

            for (int i = 0; i < this->armsCount; i++) {

                this->mean.push_back(0);
                this->N.push_back(0);
            
            }

        }

        /// <summary>
        /// Получить выигрыш
        /// </summary>
        /// <returns></returns>
        double Pull(int arm) {

            return RandFloat() + this->m[arm];

        }

        /// <summary>
        /// Обновить
        /// </summary>
        /// <param name="x">Выигрыш</param>
        /// <param name="arm">Ручка</param>
        void Update(double x, int arm) {

            this->N[arm]++;

            //Вычисление среднего выигрыша
            this->mean[arm] = (1 - 1.0f / this->N[arm]) * this->mean[arm] + 1.0f / this->N[arm] * x;

        }

        /// <summary>
        /// Получить индекс ручки, приносящей предположительно максимальный выигрыш
        /// </summary>
        /// <returns></returns>
        int GetIndexByMaxValue() {

            int index = 0;
            double value = this->mean[index];

            for (int i = 0; i < this->armsCount; i++) {

                double currValue = this->mean[i];

                if (currValue > value) {

                    value = currValue;
                    
                    index = i;

                }

            }

            return index;

        }

    private:

        /// <summary>
        /// Среднее значение всех выигрышей
        /// </summary>
        vector<double> mean;

        /// <summary>
        /// Истинное значение выигрыша
        /// </summary>
        vector<double> m;

        /// <summary>
        /// Количество экспериментов
        /// </summary>
        vector<double> N;

};

void GreedyExp(int arms, int steps, double eps) {

    #pragma region Награды бандита

    vector<double> rewards;

    for (int i = 0; i < arms; i++) {

        rewards.push_back((double)RandFloat());

        if (rand() % 10 + 1 >= 5) {

            rewards[i] = -rewards[i];

        }

    }

    #pragma endregion

    double* data = new double[steps];

    Bandit* bandit = new Bandit(rewards);

    for (int i = 0; i < steps; i++) {

        double randVal = RandFloat();

        int armIndex = -1;

        if (randVal < eps) {

            armIndex = rand() % arms;

        }
        else {

            armIndex = bandit->GetIndexByMaxValue();

        }

        double reward = bandit->Pull(armIndex);

        bandit->Update(reward, armIndex);

        data[i] = reward;

    }

    ofstream outFile;
    outFile.open("output_eps_" + to_string(eps) + ".txt", ios_base::app);
    outFile.precision(10);

    for (int i = 0; i < steps; i++) {

        double locSum = 0.0f;

        for (int j = 0; j <= i; j++) {

            locSum += data[j];

        }

        locSum /= (i + 1);

        string preparedStr = to_string(locSum);
        replace(preparedStr.begin(), preparedStr.end(), '.', ',');

        outFile << preparedStr << endl;

    }

    outFile.close();

}

int main()
{

    int steps = 1000;
    int arms = 10;

    for (int i = 0; i < 5; i++) {

        GreedyExp(arms, steps, epsilons[i]);

    }

}