#include "rummikub.h"

#include <algorithm>

RummiKub::RummiKub()
{
  actions.push_back(&RummiKub::createGroup);
  actions.push_back(&RummiKub::createRun);

  actions.push_back(&RummiKub::addToGroup);
  actions.push_back(&RummiKub::addToRun);
}

void RummiKub::Add(Tile const &tile)
{
  hand.push_back(tile);
}

void RummiKub::Solve()
{
  std::sort(hand.begin(), hand.end(), [](const Tile& a, const Tile& b)
  {
    return a.denomination > b.denomination;
  });
  unsigned start = 0;
  recursiveSolution(start);
}

std::vector<std::vector<Tile>> RummiKub::GetGroups() const
{
  return groups;
}

std::vector<std::vector<Tile>> RummiKub::GetRuns() const
{
  std::vector<std::vector<Tile>> conversion;
  conversion.resize(runs.size());

  for (unsigned i = 0; i < runs.size(); ++i)
  {
    //auto setIter = runs[i].begin();
    unsigned count = 0;
    conversion[i].resize(runs[i].size());
    for (auto itr = runs[i].begin(); itr != runs[i].end(); ++itr)
    {
      conversion[i][count++] = *itr;
    }
  }

  return conversion;
}

bool RummiKub::recursiveSolution(unsigned &index)
{
  // base case
  // out of tiles - check if runs and groups are legal
  // return true if yes and exit
  //unsigned oldIndex = index++;
  
  if (index == hand.size())
  {
    //long unsigned int count = 0;
    //for (unsigned i = 0; i < groups.size(); ++i)
    //{
    //  //std::cout << "group size at " << i << " is " << groups[i].size() << std::endl;
    //  //for(unsigned j = 0; j < groups[i].size(); ++j)
    //  //{
    //  //  std::cout << "Tile at index: " << j << " is " << groups[i][j] << std::endl;
    //  //}
    //  count += groups[i].size();
    //}

    //std::cout << "=============" << std::endl;
    //for (unsigned i = 0; i < runs.size(); ++i)
    //{
    //  std::cout << "run size at " << i << " is " << runs[i].size() << std::endl;
    //  auto tile = runs[i].begin();
    //  for(unsigned j = 0; j < runs[i].size(); ++j)
    //  {
    //    std::cout << "Tile at index: " << j << " is " << *(tile++) << std::endl;
    //  }
    //  count += runs[i].size();
    //}
    //std::cout <<"Total: " << count << '\n' << std::endl;

    return validateSolution();
  }
  //unsigned oldIndex = index++;
  ++index;
  for (bool (RummiKub::*action)(unsigned &index) : actions) {
    //if action is legal execute it
    //std::cout << "index is " << index << std::endl;
    if ((this->*action)(index))
    {
      return true;
    }
  }
  --index;

  return false; // could not find place for current tile - backtrack
}

bool RummiKub::addToGroup(unsigned &index)
{
  Tile tile = hand[index - 1];
  long long unsigned int savedSize = groups.size();
  for (unsigned i = 0; i < savedSize; ++i)
  {
    //std::vector<Tile> &group = groups[i];
    if (groups[i][0].denomination == tile.denomination)
    {
      groups[i].push_back(tile);

      if (recursiveSolution(index))
      {
        return true;
      }
      else
      {
        //std::cout << "GROUP SIZE: " << group.size() << std::endl;
        groups[i].pop_back();
      }
    }
  }
  return false;
}

bool RummiKub::addToRun(unsigned &index)
{
  Tile tile = hand[index - 1];
  long long unsigned int savedSize = runs.size();
  for (unsigned i = 0; i < savedSize; ++i)
  {
    //std::set<Tile, SortRuns> &run = runs[i];
    if (tile.color == runs[i].begin()->color)
    {
      //std::cout << "RUN BEGIN: " << run.begin()->denomination << std::endl;
      //std::cout << "RUN END: " << (--run.end())->denomination << std::endl;

      if (tile.denomination == runs[i].begin()->denomination - 1
       || tile.denomination == (--runs[i].end())->denomination + 1)
      {
        //std::cout << "Inserting" << std::endl;
        runs[i].insert(tile);
        if (recursiveSolution(index))
        {
          return true;
        }
        else
        {
          //std::cout << "Erasing" << std::endl;
          //std::cout << *(pair.first) << std::endl;
          runs[i].erase(tile);
        }
      }
    }
  }
  return false;
}

bool RummiKub::createRun(unsigned &index)
{
  std::set<Tile, SortRuns> newRun;
  newRun.insert(hand[index - 1]);

  runs.push_back(newRun);

  if (recursiveSolution(index))
  {
    return true;
  }
  else
  {
    runs.pop_back();
  }
  return false;
}

bool RummiKub::createGroup(unsigned &index)
{
  std::vector<Tile> newGroup;
  newGroup.push_back(hand[index - 1]);

  groups.push_back(newGroup);

  if (recursiveSolution(index))
  {
    return true;
  }
  else
  {
    groups.pop_back();
  }
  return false;
}

bool RummiKub::validateSolution()
{
  for (std::vector<Tile> group : groups)
  {
    if (group.size() < 3 || group.size() > 4)
    {
      return false;
    }
  }
  for (std::set<Tile, SortRuns> run : runs)
  {
    //std::cout << "RUN SIZE :" << run.size() << std::endl;
    if (run.size() < 3)
    {
      //std::cout << "e" << std::endl;
      return false;
    }
  }
  return true;
}

