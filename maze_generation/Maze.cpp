//
// Created by drgio on 07/08/2023.
//

#include "Maze.h"

namespace mgen {
    Maze::Maze(): Maze({}, 0, 0, 1.0f) {}
    Maze::Maze(std::vector<std::vector<char>>const& matrix, int rowCount, int colCount, float unit): _width(colCount), _height(rowCount){
        _unit = unit;
        for(int i = 0; i < _height; i++) {
            _matrix.emplace_back();
            for (int j = 0; j < _width; j++) {
                std::map<Direction const *, char> neighbours;
                for (auto const *direction: Direction::directions) {
                    if (*direction != Direction::TOP && *direction != Direction::BOTTOM) {
                        auto neighbourPosition = Position(j, i, 0) + *direction;
                        if (isValidPosition(neighbourPosition))
                            neighbours.insert({direction, matrix[neighbourPosition.getRow()][neighbourPosition.getCol()]});
                    }
                }
            _matrix[i].emplace_back(std::make_unique<MazeCell>(matrix[i][j], neighbours, Position(j, i,0)));
            }
        }
    }

    void Maze::buildBoxes() {
        //build north-south boxes
        for(Position startSlide = Position(0,0,0); isValidPosition(startSlide); startSlide = startSlide + Direction::EAST){
            auto slices = getSlices(startSlide, Direction::NORTH);
            for(auto const& slice : slices){
                auto box = std::make_unique<FragmentedBox>(slice[0]->getPosition(), Direction::NORTH, slice.size(), _unit);
                for(int i = 0; i < slice.size(); i++){
                    if(i > 0){
                        slice[i]->addBoxToNeighbours(Direction::SOUTH, *box.get());
                    }

                    if(i < slice.size() - 1){
                        slice[i]->addBoxToNeighbours(Direction::NORTH, *box.get());
                    }
                }

                _boxes.emplace_back(std::move(box));
            }
        }

        //build east-west boxes
        for(Position startSlide = Position(0,0,0); isValidPosition(startSlide); startSlide = startSlide + Direction::NORTH){
            auto slices = getSlices(startSlide, Direction::EAST);
            for(auto const& slice : slices){
                auto box = std::make_unique<FragmentedBox>(slice[0]->getPosition(), Direction::EAST, slice.size(), _unit);
                for(int i = 0; i < slice.size(); i++){
                    if(i > 0){
                        slice[i]->addBoxToNeighbours(Direction::WEST, *box.get());
                    }

                    if(i < slice.size() - 1){
                        slice[i]->addBoxToNeighbours(Direction::EAST, *box.get());
                    }
                }
                _boxes.emplace_back(std::move(box));
            }
        }

        //build single boxes
        for(int i = 0; i < _height; i++)
            for(int j = 0; j < _width; j++){
                if(_matrix[i][j]->isBlock() && _matrix[i][j]->getValidDirections().empty())
                    _boxes.emplace_back(std::make_unique<FragmentedBox>(Position(j, i, 0), Direction::NORTH, 1, _unit));
            }
    }

    void Maze::deleteUselessFaces(){
        for(int i = 0; i < _matrix.size(); i++)
            for(int j = 0; j < _matrix[0].size(); j++){
                auto const& cell = getCell(Position(j, i, 0));
                auto neighbourBoxes = cell.getNeighbourBoxes();
                if((neighbourBoxes.find(&Direction::NORTH) != neighbourBoxes.end() || neighbourBoxes.find(&Direction::SOUTH) != neighbourBoxes.end())
                   &&
                   (neighbourBoxes.find(&Direction::EAST) != neighbourBoxes.end() || neighbourBoxes.find(&Direction::WEST) != neighbourBoxes.end())
                        ) {
                    auto cellPosition = cell.getPosition();
                    for (auto const *side: Direction::directions) {
                        if (*side != Direction::TOP && *side != Direction::BOTTOM) {
                            auto boxPerpendicularToCurrentSide = (neighbourBoxes.find(side) != neighbourBoxes.end()) ? neighbourBoxes.find(side)->second : neighbourBoxes.find(&side->getOpposite())->second;
                            auto boxAlongCurrentSide = (neighbourBoxes.find(&side->getPerpendicularTo(Direction::TOP)) != neighbourBoxes.end())? neighbourBoxes.find(&side->getPerpendicularTo(Direction::TOP))->second : neighbourBoxes.find(&side->getPerpendicularTo(Direction::TOP).getOpposite())->second;
                            if (neighbourBoxes.find(side) != neighbourBoxes.end()) {
                                boxAlongCurrentSide->deleteFace(cellPosition, *side);
                                boxPerpendicularToCurrentSide->deleteFace(cellPosition, *side);
                            }
                            else if(neighbourBoxes.find(&side->getOpposite()) != neighbourBoxes.end())
                                boxPerpendicularToCurrentSide->deleteFace(cellPosition, *side);
                        }
                        else if(*side == Direction::TOP){
                            auto northSouthBox = (neighbourBoxes.find(&Direction::NORTH) != neighbourBoxes.end()) ? neighbourBoxes.find(&Direction::NORTH)->second : neighbourBoxes.find(&Direction::SOUTH)->second;
                            auto eastWestBox = (neighbourBoxes.find(&Direction::EAST) != neighbourBoxes.end()) ? neighbourBoxes.find(&Direction::EAST)->second : neighbourBoxes.find(&Direction::WEST)->second;
                            if(neighbourBoxes.find(&Direction::NORTH) != neighbourBoxes.end() && neighbourBoxes.find(&Direction::SOUTH) != neighbourBoxes.end()){
                                eastWestBox->deleteFace(cellPosition, Direction::TOP);
                            }else{
                                northSouthBox->deleteFace(cellPosition, Direction::TOP);
                            }
                        }
                    }
                }
            }
    }

    std::vector<FragmentedBox const*> Maze::getBoxes () const{
        auto res = std::vector<FragmentedBox const*>();
        for(auto const& box : _boxes){
            res.emplace_back(box.get());
        }

        return res;
    }

    bool Maze::isValidPosition(Position position) const{
        return position.getRow() >= 0 && position.getRow() < _height && position.getCol() >= 0 && position.getCol() < _width;
    }

    bool Maze::isBlock(Position position) const{
        return isValidPosition(position) && getCell(position).isBlock();
    }

    bool Maze::isEmpty(Position position) const{
        return isValidPosition(position) && !isBlock(position);
    }

    MazeCell& Maze::getCell(Position position) const{
        if(!isValidPosition(position)){
            std::ostringstream oss;
            oss << "Position (" << position.getCol() << ", " << position.getRow() << ", " << 0 << ") is out of bound" << std::endl;
            throw std::runtime_error(oss.str());
        }
        return *_matrix[position.getRow()][position.getCol()];
    }

    std::vector<std::vector<MazeCell*>> Maze::getSlices(Position startPosition, Direction const& direction) const{
        auto slice = std::vector<std::vector<MazeCell*>>{};
        Position currentStartSlicePosition = startPosition;
        int sliceIndex = -1;

        while(isValidPosition(currentStartSlicePosition)) {
            //find first solid block
            while (isValidPosition(currentStartSlicePosition)) {
                auto validDirections = getCell(currentStartSlicePosition).getValidDirections();
                if(validDirections.find(&direction) != validDirections.end() || validDirections.find(&direction.getOpposite()) != validDirections.end())
                    break;
                currentStartSlicePosition = currentStartSlicePosition + direction;
            }
            if (!isValidPosition(currentStartSlicePosition))
                return slice;
            slice.emplace_back();
            sliceIndex++;
            slice[sliceIndex].emplace_back(&getCell(currentStartSlicePosition));

            //find last consecutive solid block
            Position currentEndSlicePosition = currentStartSlicePosition + direction;
            while (isValidPosition(currentEndSlicePosition)) {
                auto validDirections = getCell(currentEndSlicePosition).getValidDirections();
                if(validDirections.find(&direction) == validDirections.end() && validDirections.find(&direction.getOpposite()) == validDirections.end())
                    break;
                slice[sliceIndex].emplace_back(&getCell(currentEndSlicePosition));
                currentEndSlicePosition = currentEndSlicePosition + direction;
            }
            currentStartSlicePosition = currentEndSlicePosition;
        }

        return slice;
    }

} // mgen