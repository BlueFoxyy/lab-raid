# lab-raid

## WORK IN PROGRESS

Sorry for using this as a note lol

## Notes
- Graphics
	- Use low alpha with additive blending for flashlights. Color: #FFDC80.
	- Shading: use multiply blending mode.
- Add support for rendering a single object/shape in renderer.
- Add support for rendering text in renderer.
- Add support for object hitbox and collision.
	- Implement interpolation for object collision
- Rendering:
	- Add layer priorities for object rendering. (Hint: `std::pair<RenderPriority, std::weak_ptr<RenderObjectBase>>`).

### Reminder: Check TODO comments in code.