import { Coordinate } from './coordinate';
import { Wall, Direction } from './wall';

export class Room {
    walls: Wall[];
    forwardWalls: Wall[] = [];
    backwardWalls: Wall[] = [];
    rightWalls: Wall[] = [];
    leftWalls: Wall[] = [];

    constructor(walls: Wall[]) {
        this.walls = walls;

        this.walls.forEach(wall => {
            switch (wall.direction) {
                case Direction.Forward:
                    this.forwardWalls.push(wall);
                    break;
                case Direction.Right:
                    this.rightWalls.push(wall);
                    break;
                case Direction.Backward:
                    this.backwardWalls.push(wall);
                    break;
                case Direction.Left:
                    this.leftWalls.push(wall);
                    break;
            }
        });
    }

    draw() {
        this.walls.forEach(wall => {
            wall.extend(20);
        });
    }

    detectIntersections() {
        //Foreach wall
        // L, R = Find closest intersecting F, B (vice versa)
        // Edit both paths to make them meet at intersecting coordinate

        this.walls.forEach(wall => {
            if (wall.direction == Direction.Forward || wall.direction == Direction.Backward) {
                const rightIntersections: Wall[] = [];
                const leftIntersections: Wall[] = [];

                this.rightWalls.forEach(rightWall => {
                    if (wall.path.intersects(rightWall.path)) {
                        rightIntersections.push(rightWall);
                    }
                });

                this.leftWalls.forEach(leftWall => {
                    if (wall.path.intersects(leftWall.path)) {
                        leftIntersections.push(leftWall);
                    }
                });

                var closestRight = wall.findClosestWall(rightIntersections);
                var closestLeft = wall.findClosestWall(leftIntersections);

                var rightIntersectingPoint = wall.path.getIntersections(closestRight.path)[0].point;
                var leftIntersectingPoint = wall.path.getIntersections(closestLeft.path)[0].point;

                if (wall.coordinates[0].x > wall.coordinates[1].x) {
                    // First coordinate is on the right, second left
                    wall.coordinates[0] = new Coordinate(
                        rightIntersectingPoint.x,
                        rightIntersectingPoint.y
                    );

                    wall.coordinates[1] = new Coordinate(
                        leftIntersectingPoint.x,
                        leftIntersectingPoint.y
                    );
                } else {
                    // First coordinate is on the left, second right
                    wall.coordinates[0] = new Coordinate(
                        leftIntersectingPoint.x,
                        leftIntersectingPoint.y
                    );

                    wall.coordinates[1] = new Coordinate(
                        rightIntersectingPoint.x,
                        rightIntersectingPoint.y
                    );
                }

            } else {
                const forwardIntersections: Wall[] = [];
                const backwardIntersections: Wall[] = [];

                this.forwardWalls.forEach(forwardWall => {
                    if (wall.path.intersects(forwardWall.path)) {
                        forwardIntersections.push(forwardWall);
                    }
                });

                this.backwardWalls.forEach(backwardWall => {
                    if (wall.path.intersects(backwardWall.path)) {
                        backwardIntersections.push(backwardWall);
                    }
                });

                var closestForward = wall.findClosestWall(forwardIntersections);
                var closestBackward = wall.findClosestWall(backwardIntersections);

                var forwardIntersectingPoint = wall.path.getIntersections(closestForward.path)[0].point;
                var backwardIntersectingPoint = wall.path.getIntersections(closestBackward.path)[0].point;

                if (wall.coordinates[0].y > wall.coordinates[1].y) {
                    // First coordinate is on the back, second front
                    wall.coordinates[0] = new Coordinate(
                        backwardIntersectingPoint.x,
                        backwardIntersectingPoint.y
                    );

                    wall.coordinates[1] = new Coordinate(
                        forwardIntersectingPoint.x,
                        forwardIntersectingPoint.y
                    );
                } else {
                    // First coordinate is on the front, second back
                    wall.coordinates[0] = new Coordinate(
                        forwardIntersectingPoint.x,
                        forwardIntersectingPoint.y
                    );

                    wall.coordinates[1] = new Coordinate(
                        backwardIntersectingPoint.x,
                        backwardIntersectingPoint.y
                    );
                }
            }
        });

        console.log("Redrawing Walls!")
        this.walls.forEach(wall => {
            wall.draw();
        });
    }
}
