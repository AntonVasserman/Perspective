# Perspective Metrics Doc

## 3Cs Metrics

| Parameter                         | Value |
|-----------------------------------|-------|
| Character Collision Height        | 200   |
| Character Crouch Collision Height | 150   |
| Character Walkable Angel          | 46.0  |
| Max Fall Height                   | 1100m |

## Level Metrics
### Environment
| Parameter                                       | Value                                 |
|-------------------------------------------------|---------------------------------------|
| Floor Distance with Assets for Character Crouch | 150                                   |
| Ramp Distance with Assets for Character Crouch  | 200                                   |
| Floor Asset Scale                               | (1.25, 1.25, 0.25)                    |
| Floor Asset Scale and Rotation as 500x250 ramp  | ((1.3955, 1.25, 0.25), (0, -26.6, 0)) |
| Floor Asset Scale and Rotation as 750x250 ramp  | ((1.25, 1.9765, 0.25), (0, 18.5, 0))  |

### Mechanics in each Level Instance

| Level | List of mechanics                | Additional Info                             |
|-------|----------------------------------|---------------------------------------------|
| 00    | Button → Sliding Door            | Control Introduction, Mechanic Introduction |
|       | Button → Sliding Door with Timer |                                             |
| 01    | Button → Sliding Door            | Puzzle                                      |
|       | Button → Rotating Door           |                                             |
| 02    | Button → Sliding Door            | Puzzle                                      |
|       | Button → Rotating Door           |                                             |
| 03    | Button → Sliding Door            | Mechanic Introduction & Puzzle              |
|       | Rect Gate                        |                                             |
| 04    | Button → Platform                | Mechanic Introduction & Puzzle              |
|       | Button → Rect Gate               |                                             |
|       | Rect Gate → Only 2D Button       |                                             |


