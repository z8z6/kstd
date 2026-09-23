# Runtime reflection

`@reflect` selects fields at compile time. On a class it selects all public
instance fields and any explicitly annotated private instance fields; on a
field alone it selects just that field. Unannotated fields remain absent from
runtime metadata.

```kelyra
import std.reflect;

@reflect class Player {
  pub health: i32;
  @reflect private_id: u64;
}

let player = Player();
let info = std.reflect.type_of<Player>();
if info.has_field("health") {
  let found = info.field("health"); // Option<FieldInfo>
  if found.ok {
    let field = found.value();
    let field_type = field.type();  // TypeInfo: id and name
    let object = std.reflect.value<Player>(&player);
    let previous = std.reflect.read<i32>(field, object);
    let written = std.reflect.write<i32>(field, object, 80);
  }
}
```

`Option<T>.err` is `0` for an absent field and nonzero for an error. Runtime
read and write use error codes `1` (wrong owner type), `2` (wrong field type),
and `3` (null object pointer). Allocation failure is `12`. Check `ok` before
calling `value()`. `write` uses normal class assignment, including copy/move
and destruction; `read` returns a copy of the field.

`TypeInfo.field_count()` counts selected fields. `FieldInfo.type()` currently
returns the field type's identity and name, not a recursive field table.
`type_of<T>()` provides a field table only if T itself is annotated.

The compiler emits one read-only descriptor in the object file that defines
each reflected class. Consumers can use `type_of<T>()` while linking a
precompiled library; they still need the class declaration source for static
type checking. No C implementation or host CRT is required.

Descriptor ABI v1 uses an exported `__kelyra_reflect_v1_...` symbol per type.
Its byte header contains `KLRF`, version `1`, a 64-bit type ID, the type-name
length, and the selected-field count. Each field stores its type ID, byte
offset, name length, type-name length, and null-terminated names. The standard
library checks the magic and version before reading a descriptor. Rebuild a
library after an incompatible compiler ABI change.

Reflection metadata is emitted for fields, not methods or annotations. This
API is separate from the general `@retention(runtime)` annotation ABI draft.
