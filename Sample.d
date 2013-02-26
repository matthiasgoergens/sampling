#!/usr/bin/env rdmd
import std.stdio;
import std.random;
import std.math;
import std.container;

double geom (double p) {
  return floor (log (1-uniform(0.0L, 1.0L)) / log (1-p));
  // transform p x = floor $ log x / log (1-p)  
}

void main() {
  /*  ulong lines = 0;
  double sumLength = 0;
  foreach (line; stdin.byLine()) {
    ++lines; sumLength += line.length;
  }
  writeln("Average line length: ", lines ? sumLength / lines : 0); */
  auto i = uniform(0, 15);
  writeln (i);


  // Random gen;
  auto r = uniform(0.0L, 1.0L);
  auto s = [(1,"bla"), (2, "blub"), (1, "ou")];
  writeln(geom(0.5));
  writeln(s[0]);

  double[] a = [ 4.0, 1.0, 3.0, 2.0, 16.0, 9.0, 10.0, 14.0, 8.0, 7.0 ]; auto h = heapify(s);
  writeln(h);
}

