import { Direction } from './../../classes/wall';
import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { PaperScope, Project, Path, Point, view, Matrix, Rectangle, Size } from 'paper';
import { Coordinate } from 'src/app/classes/coordinate';
import { Wall } from 'src/app/classes/wall';
import { Room } from '../../classes/room';

@Component({
  selector: 'app-paper-canvas',
  templateUrl: './paper-canvas.component.html',
  styleUrls: ['./paper-canvas.component.css']
})
export class PaperCanvasComponent implements OnInit {
  @ViewChild('canvasElement') canvasElement: ElementRef;
  scope: PaperScope;
  project: Project;

  constructor() { }

  ngOnInit() {
    this.scope = new PaperScope();
    this.project = new Project(this.canvasElement.nativeElement);

    // Mark center
    const centerMarker = new Path.Rectangle(
      new Rectangle(new Point(this.scope.view.center.x - 1, this.scope.view.center.x - 1), new Size(2, 2))
    );
    centerMarker.fillColor = 'red';

    // Draw walls
    const origin = new Coordinate(this.scope.view.center.x, this.scope.view.center.y);
    const walls = [
      new Wall(origin, Direction.Forward, [20, 20]),
      new Wall(origin, Direction.Backward, [20, 20]),
      new Wall(origin, Direction.Right, [20, 20]),
      new Wall(origin, Direction.Left, [20, 20]),
    ];

    /*
    // Path setup
    const path = new Path();
    path.strokeColor = 'black';

    // Create Points from Coordinates
    const start = new Point(240,240);
    const end = new Point(260, 260);

    // Draw line
    path.moveTo(start);
    path.lineTo(end);
    */

    const room = new Room(walls);
    room.draw();
    
    console.log(room.walls[0].path.intersects(room.walls[3].path));
    
  }

}
