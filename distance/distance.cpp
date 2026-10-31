#include <iostream>
#include <vector>
using namespace std;

const int INF = 999;

void displayRoutingTable(int router, vector<vector<int>> &dist,
                         vector<vector<int>> &nextHop)
{
    int n = dist.size();

    cout << "\nRouter " << router + 1 << endl;
    cout << "Dest\tCost\tNext Hop" << endl;

    for (int j = 0; j < n; j++)
    {
        cout << j + 1 << "\t";

        if (dist[router][j] >= INF)
            cout << "INF\t";
        else
            cout << dist[router][j] << "\t";

        if (nextHop[router][j] == -1)
            cout << "-";
        else
            cout << nextHop[router][j] + 1;

        cout << endl;
    }
}

int main()
{
    int n;

    cout << "Enter number of routers: ";
    cin >> n;

    vector<vector<int>> cost(n, vector<int>(n));
    vector<vector<int>> dist(n, vector<int>(n));
    vector<vector<int>> nextHop(n, vector<int>(n));

    cout << "\nEnter cost matrix (999 for no connection):\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> cost[i][j];
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {

            dist[i][j] = cost[i][j];

            if (i == j)
                nextHop[i][j] = i;
            else if (cost[i][j] != INF)
                nextHop[i][j] = j;
            else
                nextHop[i][j] = -1;
        }
    }

    cout << "\nCost matrix:\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (cost[i][j] >= INF)
                cout << "INF ";
            else
                cout << cost[i][j] << "   ";
        }
        cout << endl;
    }

    cout << "\nInitial routing tables:\n";

    for (int i = 0; i < n; i++)
    {
        displayRoutingTable(i, dist, nextHop);
    }

    // Distance Vector updates
    bool updated;

    do
    {
        updated = false;

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < n; k++)
                {

                    if (cost[i][k] != INF &&
                        dist[k][j] != INF &&
                        dist[i][j] > cost[i][k] + dist[k][j])
                    {

                        dist[i][j] = cost[i][k] + dist[k][j];
                        nextHop[i][j] = k;

                        updated = true;
                    }
                }
            }
        }

    } while (updated);

    cout << "\nFinal routing tables:\n";

    for (int i = 0; i < n; i++)
    {
        displayRoutingTable(i, dist, nextHop);
    }

    int source, destination;

    cout << "\nEnter source router: ";
    cin >> source;

    cout << "Enter destination router: ";
    cin >> destination;

    source--;
    destination--;

    if (dist[source][destination] >= INF)
    {
        cout << "\nNo path found\n";
    }
    else
    {
        cout << "\nMinimum cost: "
             << dist[source][destination] << endl;

        cout << "Path: ";

        int current = source;
        cout << current + 1;

        while (current != destination)
        {
            current = nextHop[current][destination];
            cout << " -> " << current + 1;
        }

        cout << endl;
    }

    return 0;
}