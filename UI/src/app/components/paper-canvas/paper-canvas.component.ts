import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { PaperScope, Project, Path, Point } from 'paper';

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

    // Create a Paper.js Path to draw a line into it:
    const path = new Path();
    path.strokeColor = 'black';

    // Coords
    const start = new Point(100, 100);
    const end = new Point(100, -50);

    // Draw line
    path.moveTo(start);
    path.lineTo(end);
  }

}
