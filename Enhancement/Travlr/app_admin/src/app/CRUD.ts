import express, { Request, Response } from 'express';

const app = express();
app.use(express.json());

// 1. Define the Interface
interface Item {
  id: number;
  name: string;
  description?: string;
}

// In-memory data store
let items: Item[] = [];

// 2. CREATE: Add a new item
app.post('/items', (req: Request, res: Response) => {
  const newItem: Item = {
    id: Date.now(),
    name: req.body.name,
    description: req.body.description,
  };
  items.push(newItem);
  res.status(201).json(newItem);
});

// 3. READ: Get all items
app.get('/items', (req: Request, res: Response) => {
  res.json(items);
});

// 4. READ: Get a single item by ID
app.get('/items/:id', (req: Request, res: Response) => {
  const item = items.find(i => i.id === parseInt(req.params.id));
  if (!item) return res.status(404).send('Item not found');
  res.json(item);
});

// 5. UPDATE: Edit an existing item by ID
app.put('/items/:id', (req: Request, res: Response) => {
  const index = items.findIndex(i => i.id === parseInt(req.params.id));
  if (index === -1) return res.status(404).send('Item not found');

  items[index] = {
    id: parseInt(req.params.id),
    name: req.body.name || items[index].name,
    description: req.body.description || items[index].description,
  };
  res.json(items[index]);
});

// 6. DELETE: Remove an item by ID
app.delete('/items/:id', (req: Request, res: Response) => {
  const index = items.findIndex(i => i.id === parseInt(req.params.id));
  if (index === -1) return res.status(404).send('Item not found');
  
  items.splice(index, 1);
  res.status(204).send();
});

// 7. Start the server
const PORT = 3000;
app.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}`);
});

