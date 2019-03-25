import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { PaperScope, Project, Path, Point, view } from 'paper';
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
    const centerCross1 = new Path();
    centerCross1.strokeColor = 'black';
    centerCross1.moveTo(new Point(this.scope.view.center.x - 1, this.scope.view.center.y + 1));
    centerCross1.lineTo(new Point(this.scope.view.center.x + 1, this.scope.view.center.y - 1));

    const centerCross2 = new Path();
    centerCross2.strokeColor = 'black';
    centerCross2.moveTo(new Point(this.scope.view.center.x + 1, this.scope.view.center.y + 1));
    centerCross2.lineTo(new Point(this.scope.view.center.x - 1, this.scope.view.center.y - 1));


    const origin = new Coordinate(this.scope.view.center.x, this.scope.view.center.y);
    const wall = new Wall(origin, 90, [20, 20]);
    wall.draw();
  }
}
