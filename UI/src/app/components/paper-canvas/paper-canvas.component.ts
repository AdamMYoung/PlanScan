import { Direction } from './../../classes/wall';
import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { PaperScope, Project, Path, Point, view, Matrix, Rectangle, Size } from 'paper';
import { Coordinate } from 'src/app/classes/coordinate';
import { Wall } from 'src/app/classes/wall';

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

    // Draw wall
    const origin = new Coordinate(this.scope.view.center.x, this.scope.view.center.y);
    const wall = new Wall(origin, Direction.Backward, [20, 20]);
    wall.draw();
  }

}

