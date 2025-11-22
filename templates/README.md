# Templates Directory

This directory contains template images and their metadata for template matching.

## Structure

Each template consists of:
- A PNG image file (e.g., `start_button.png`)
- A JSON metadata file (e.g., `start_button.json`)

## Template Metadata Format

```json
{
  "template_id": "start_button",
  "name": "Start Button",
  "image_path": "templates/start_button.png",
  "search_region": {
    "top_left": {
      "x": 800.0,
      "y": 900.0
    },
    "bottom_right": {
      "x": 1120.0,
      "y": 1000.0
    }
  },
  "action_point": {
    "x": 960.0,
    "y": 950.0
  },
  "confidence_threshold": 0.8,
  "created_at": "2024-01-15T10:30:00Z",
  "updated_at": "2024-01-15T10:30:00Z"
}
```

## Creating Templates

Use the Template Creator tool in the Manager UI to:
1. Load a screenshot
2. Draw the search region
3. Set the action point
4. Save the template

All coordinates are normalized to 1920x1080 resolution.
