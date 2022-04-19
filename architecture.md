# Shared data
current_x, current_y, current_t

# Modules
## Scan
initGameMap()
ScanMap()
Cell2Check()

## Movements
func move(target_x, target_y, target_theta) 
    read path array from pathFinding(), use the forward, backward, left, right, stop func accordingly
func forward() : move 1/4 of a cell width forward
func backward() : move 1/4 of a cell width backward
func left() : turn 90° left
func right() : turn 90° left
func stop() : do nothing, code to tell the move func the target has been reached

## Path
type path_t = { x, y, t 
func pathFinding(target_x, target_y, target_t)
atomic movement : 1/4 of a cell
