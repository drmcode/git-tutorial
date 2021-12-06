#include "collectrain.hpp"

rainNormal::rainNormal() {}
rainNormal::~rainNormal() {}

int rainNormal::collectedRain(std::vector<int>& p_wall)
{    
    int l_wallSize = p_wall.size();
    int l_leftWallMax;
    int l_rightWallMax;
    int l_ans = 0;

    for (int i = 0; i < l_wallSize; i++)
    {
        l_leftWallMax = 0;
        l_rightWallMax = 0;

        for (int j = 0; j < i; j++)
        {
            l_leftWallMax = rainNormal::max(l_leftWallMax, p_wall[j]);
        }

        for (int k = i; k < l_wallSize; k++)
        {
            l_rightWallMax = rainNormal::max(l_rightWallMax, p_wall[k]);
        }

        if (rainNormal::min(l_leftWallMax, l_rightWallMax) > p_wall[i])
        {
            l_ans += rainNormal::min(l_leftWallMax, l_rightWallMax) - p_wall[i];
        }
    }

    return l_ans;
}

rainDp::rainDp(){}
rainDp::~rainDp(){}

int rainDp::collectedRain(std::vector<int>& p_wall)
{
     int l_wallSize = p_wall.size();
     int l_ans = 0;

     std::vector<int> l_leftWallMax(l_wallSize, 0);
     std::vector<int> l_rightWallMax(l_wallSize, 0);

     l_leftWallMax[0] = p_wall[0];
     l_rightWallMax[l_wallSize - 1] = p_wall[l_wallSize - 1];

     for (int i = 1; i < l_wallSize; i++)
     {
         l_leftWallMax[i] = rainDp::max(l_leftWallMax[i-1], p_wall[i]);
     }

     for (int j = l_wallSize - 2; j >= 0; j-- )
     {
         l_rightWallMax[j] = rainDp::max(l_rightWallMax[j+1], p_wall[j]);
     }

     for (int k = 0; k < l_wallSize; k++)
     {
         if (rainDp::min(l_leftWallMax[k], l_rightWallMax[k]) > p_wall[k])
            l_ans += rainDp::min(l_leftWallMax[k], l_rightWallMax[k]) - p_wall[k];
     }

     return l_ans;
}


rainDoublePoint::rainDoublePoint(){}
rainDoublePoint::~rainDoublePoint(){}

int rainDoublePoint::collectedRain(std::vector<int>& p_wall)
{
    int l_wallSize = p_wall.size();
    int l_ans = 0;
    int l_leftIndex = 0;
    int l_rightIndex = l_wallSize - 1;
    int l_leftWallMax = p_wall[l_leftIndex];
    int l_rightWallMax = p_wall[l_rightIndex];

    while(l_leftIndex <= l_rightIndex)
    {
         if (l_leftWallMax < l_rightWallMax)
         {
             l_leftWallMax = rainDoublePoint::max(l_leftWallMax, p_wall[l_leftIndex]);
             l_ans += l_leftWallMax - p_wall[l_leftIndex++];
         }
         else
         {
             l_rightWallMax = rainDoublePoint::max(l_rightWallMax, p_wall[l_rightIndex]);
             l_ans += l_rightWallMax - p_wall[l_rightIndex--];
         }
    }
    
    return l_ans;
}

rainStack::rainStack(){}
rainStack::~rainStack(){}
int rainStack::collectedRain(std::vector<int>& p_wall)
{
    int l_wallSize = p_wall.size();

    std::vector<int> l_wallStack;
    int l_index = 0;
    int l_ans = 0;

    std::cout<<"l_wallSize: "<<l_wallSize<<std::endl;

    while (l_index < l_wallSize)
    {
        std::cout<<"l_index: "<<l_index<<std::endl;
        while (!l_wallStack.empty() && p_wall[l_wallStack.back()] < p_wall[l_index])
        {
            int l_indexBack = l_wallStack.back();
            l_wallStack.pop_back();
            if (l_wallStack.empty())
            {
                std::cout<<"l_wallStack empty break;"<<std::endl;
                break;
            }

            std::cout<<"l_indexBack: "<<l_indexBack<<std::endl;
            int l_distance = l_index - l_wallStack.back() - 1;
            int l_high = rainStack::min(p_wall[l_index], p_wall[l_wallStack.back()]) - p_wall[l_indexBack];
            l_ans += l_distance * l_high;
            std::cout<<"l_ans: "<<l_ans<<", l_distance: "<<l_distance<<", l_high: "<<l_high<<std::endl;
        }
        l_wallStack.push_back(l_index++);
    }
    
    return l_ans;
}
