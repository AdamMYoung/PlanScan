import { Coordinate } from './coordinate';
import { Path, Point, PointText } from 'paper';
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
        /*
        for (var i = 0; i < 20; i++) {
            this.walls.forEach(wall => {
                wall.extend(10);
            });
        }
        */

        this.walls.forEach(wall => {
            wall.extend(20);
        });

    }

    detectIntersection() {
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
                        rightIntersections.push(leftWall);
                    }
                });

                var closestRight = wall.findClosestWall(rightIntersections);
                var closestLeft = wall.findClosestWall(leftIntersections);

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
            }
        });
    }
}
